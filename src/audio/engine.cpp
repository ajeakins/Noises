
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <iostream>

#include <QSet>

#include <application.h>

#include "engine.h"

#define SAMPLE_RATE			(44100)
#define FRAMES_PER_BUFFER	(512)

namespace noises
{
namespace audio
{

PaStream* Engine::m_stream;
int Engine::m_channel_count = 0;

void onError( const PaError& error )
{
	Pa_Terminate();
	std::cout << "An error occured while using the portaudio stream" << std::endl;
	std::cout << "Error number: " << error << std::endl;
	std::cout << "Error message: " << Pa_GetErrorText( error ) << std::endl;
}

Engine::Engine( QObject* parent )
:
	QObject( parent )
{
	PaError error;

	error = Pa_Initialize();
	if( error != paNoError )
	{
		onError( error );
		return;
	}
}

Engine::~Engine()
{
	Pa_Terminate();
}

void Engine::registerPlayer( Player::Ptr player )
{
	QMutexLocker( &m_playback_data.m_lock );
	m_playback_data.m_players.append( player );
}

void Engine::stop()
{
	if ( m_stream )
	{
		Pa_StopStream( m_stream );
	}

	// stop the players
	PlayerList::iterator itr = m_playback_data.m_players.begin();
	for ( ; itr !=  m_playback_data.m_players.end(); ++itr )
	{
		( **itr ).stop();
	}

	m_playback_data.m_players.clear();
}

void Engine::run()
{
	setup();

	Q_EMIT finished();
}

void Engine::setup()
{
	PaError error;

	PaStreamParameters outputParameters;
	outputParameters.device = Pa_GetDefaultOutputDevice();
	if ( outputParameters.device == paNoDevice )
	{
		std::cout << "Error: No default output device." << std::endl;
		return;
	}

	m_channel_count = Application::getPreferences().getOutputCount();

	outputParameters.channelCount = m_channel_count;
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	error = Pa_OpenStream(
		&m_stream,
		NULL, /*no input*/
		&outputParameters,
		SAMPLE_RATE,
		paFramesPerBufferUnspecified /*frames per buffer*/,
		paNoFlag,
		Engine::audioCallback,
		&m_playback_data );

	if( error != paNoError )
	{
		onError( error );
		return;
	}

	error = Pa_StartStream( m_stream );
	if( error != paNoError )
	{
		onError( error );
		return;
	}

	// check every 100ms
	while( Pa_IsStreamActive( m_stream ) == 1 )
	{
		Pa_Sleep( 100 );
	}

	error = Pa_CloseStream( m_stream );
	if( error != paNoError )
	{
		onError( error );
		return;
	}
}

int Engine::audioCallback(
	const void* /*inputBuffer*/,
	void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* /*timeInfo*/,
	PaStreamCallbackFlags /*statusFlags*/,
	void* userData )
{
	PlaybackData* playback_data = ( PlaybackData* )userData;
	QMutexLocker( &playback_data->m_lock );

	float* out = ( float* )outputBuffer;
	std::fill( out, out + ( framesPerBuffer * m_channel_count ), 0.0f );

	QMutableListIterator< Player::Ptr > itr( playback_data->m_players );
	while ( itr.hasNext() )
	{
		Player::Ptr player = itr.next();
		player->addData( out, framesPerBuffer, m_channel_count );

		if ( !player->isPlaying() )
		{
			itr.remove();
		}
	}

	if ( playback_data->m_players.isEmpty() )
	{
		return paComplete;
	}

	return paContinue;
}

} /* namespace audio */
} /* namespace noises */


#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <iostream>

#include <app/application.h>

#include "engine.h"

#define FRAMES_PER_BUFFER	(512)

namespace noises
{
namespace audio
{

namespace
{
	void onError( const PaError& error )
	{
		Pa_Terminate();
		std::cout << "An error occured while using the portaudio stream" << std::endl;
		std::cout << "Error number: " << error << std::endl;
		std::cout << "Error message: " << Pa_GetErrorText( error ) << std::endl;
	}
}

PaStream* Engine::m_stream;
int Engine::m_channel_count = 0;

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
	QMutexLocker lock( &m_players_lock );
	m_players.append( player );
}

void Engine::registerPlayers( QList< Player::Ptr >& players )
{
	QMutexLocker lock( &m_players_lock );
	m_players.append( players );
}

void Engine::stop()
{
	if ( m_stream )
	{
		Pa_StopStream( m_stream );
	}

	for ( Player::Ptr player : m_players )
	{
		player->stop();
	}

	m_players.clear();
}

void Engine::start()
{
	PaError error;

	PaStreamParameters outputParameters;
	outputParameters.device = Pa_GetDefaultOutputDevice();
	if ( outputParameters.device == paNoDevice )
	{
		std::cout << "Error: No default output device." << std::endl;
		return;
	}

	Preferences& prefs = Application::getPreferences();
	m_channel_count = prefs.getOutputCount();

	outputParameters.channelCount = m_channel_count;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	error = Pa_OpenStream(
		&m_stream,
		NULL, /*no input*/
		&outputParameters,
		prefs.getSampleRate(),
		/*paFramesPerBufferUnspecified*/ FRAMES_PER_BUFFER,
		paNoFlag,
		Engine::audioCallback,
		this );

	if( error != paNoError )
	{
		onError( error );
		return;
	}

	Pa_SetStreamFinishedCallback(m_stream, Engine::finishedCallback);

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

	is_running = true;
}

int Engine::audioCallback(
	const void* /*inputBuffer*/,
	void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* /*timeInfo*/,
	PaStreamCallbackFlags /*statusFlags*/,
	void* userData )
{
	Engine* engine = static_cast< Engine* >( userData );
	QMutexLocker( &engine->m_players_lock );

	float* out = ( float* )outputBuffer;
	std::fill( out, out + ( framesPerBuffer * m_channel_count ), 0.0f );

	QMutableListIterator< Player::Ptr > itr( engine->m_players );
	while ( itr.hasNext() )
	{
		Player::Ptr player = itr.next();
		player->addData( out, framesPerBuffer, m_channel_count );

		if ( !player->isPlaying() )
		{
			itr.remove();
		}
	}

	if ( engine->m_players.isEmpty() )
	{
		return paComplete;
	}

	return paContinue;
}

void Engine::finishedCallback( void* userData )
{
	Engine* engine = static_cast< Engine* >( userData );
	engine->is_running = false;
	Q_EMIT engine->finished();
}

} /* namespace audio */
} /* namespace noises */

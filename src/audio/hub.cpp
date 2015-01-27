
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <iostream>

#include <QSet>

#include "hub.h"

#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (512)

namespace noises
{
namespace audio
{

PaStream* Hub::stream;

void onError( const PaError& error )
{
	Pa_Terminate();
	std::cout << "An error occured while using the portaudio stream" << std::endl;
	std::cout << "Error number: " << error << std::endl;
	std::cout << "Error message: " << Pa_GetErrorText( error ) << std::endl;
}

Hub::Hub( QObject* parent )
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

Hub::~Hub()
{
	Pa_Terminate();
}

void Hub::registerPlayer( Player::Ptr player )
{
	QMutexLocker( &m_playback_data.m_lock );
	m_playback_data.m_players.append( player );
}

void Hub::stop()
{
	std::cout << "Hub::stop" << std::endl;
	if ( stream )
	{
		Pa_StopStream( stream );
	}

	// stop the players
	PlayerList::iterator itr = m_playback_data.m_players.begin();
	for ( ; itr !=  m_playback_data.m_players.end(); ++itr )
	{
		( **itr ).stop();
	}

	m_playback_data.m_players.clear();
}

void Hub::run()
{
	setup();

	Q_EMIT finished();
}

void Hub::setup()
{
	PaError error;

	PaStreamParameters outputParameters;
	outputParameters.device = Pa_GetDefaultOutputDevice();
	if ( outputParameters.device == paNoDevice )
	{
		std::cout << "Error: No default output device." << std::endl;
		return;
	}

	outputParameters.channelCount = 2;       /* stereo output */
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	error = Pa_OpenStream(
		&stream,
		NULL, /* no input */
		&outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paNoFlag,
		Hub::audioCallback,
		&m_playback_data );

	if( error != paNoError )
	{
		onError( error );
		return;
	}

	error = Pa_StartStream( stream );
	if( error != paNoError )
	{
		onError( error );
		return;
	}

	// check every 100ms
	while( Pa_IsStreamActive( stream ) == 1 )
	{
		Pa_Sleep( 100 );
	}

	error = Pa_CloseStream( stream );
	if( error != paNoError )
	{
		onError( error );
		return;
	}

	std::cout << "done" << std::endl;
}

void Hub::streamFinished( void* /*userData*/ )
{
	std::cout << "Stream Completed:" << std::endl;
}

int Hub::audioCallback(
	const void* /*inputBuffer*/,
	void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* /*timeInfo*/,
	PaStreamCallbackFlags /*statusFlags*/,
	void *userData )
{
	//std::cout << "audioCallback" << std::endl;

	typedef QSet< Player::Ptr > PlayerSet;

	PlaybackData* playback_data = ( PlaybackData* )userData;
	float* out = ( float* )outputBuffer;

	QMutexLocker( &playback_data->m_lock );

	PlayerSet players_to_remove;

	// TODO: take into account channels...
	for( unsigned long i = 0; i != framesPerBuffer; ++i )
	{
		float left = 0.0f;
		float right = 0.0f;

		for ( int j = 0; j != playback_data->m_players.size(); ++j )
		{
			Player::Ptr player = playback_data->m_players[j];

			left += player->data();
			right += player->data();

			if ( !player->isPlaying() )
			{
				players_to_remove.insert( player );
			}
		}

		*out++ = left;
		*out++ = right;
	}

	if ( players_to_remove.size() == playback_data->m_players.size() )
	{
		playback_data->m_players.clear();
	}
	else if ( !players_to_remove.isEmpty() )
	{
		PlayerSet::const_iterator itr = players_to_remove.begin();
		for ( ; itr != players_to_remove.end(); ++itr )
		{
			playback_data->m_players.removeOne( *itr );
		}
	}

	if ( playback_data->m_players.isEmpty() )
	{
		std::cout << "paComplete" << std::endl;
		return paComplete;
	}

	std::cout << "paContinue" << std::endl;
	return paContinue;
}

} /* namespace audio */
} /* namespace noises */

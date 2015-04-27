
#include <assert.h>
#include <iostream>

#include <QTimer>
#include <QReadWriteLock>

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

Engine::Engine( QObject* parent )
:
	QObject( parent ),
	m_players_lock( new QReadWriteLock ),
	m_player_update_timer( new QTimer( this ) )
{
	PaError error;

	error = Pa_Initialize();
	if( error != paNoError )
	{
		onError( error );
		return;
	}

	// This timer is evaluated in the GUI event loop
	// rather than using strict playback time but
	// the values written to the GUI are playback time.
	// 50ms seems like the sweet spot for smooth updates.
	m_player_update_timer->setInterval( 50 );
	connect( m_player_update_timer, &QTimer::timeout,
			this, &Engine::updatePlayerTimes );
}

Engine::~Engine()
{
	Pa_Terminate();
	delete m_players_lock;
}

void Engine::registerPlayer( Player::Ptr player )
{
	QWriteLocker lock( m_players_lock );
	m_players.append( player );
}

void Engine::registerPlayers( QList< Player::Ptr >& players )
{
	QWriteLocker lock( m_players_lock );
	m_players.append( players );
}

void Engine::updatePlayerTimes()
{
	QReadLocker lock( m_players_lock );
	if ( !m_players.isEmpty() )
	{
		for ( Player::Ptr player : m_players )
		{
			player->updateTime();
		}
	}
	else
	{
		m_player_update_timer->stop();
	}
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
		player->updateTime();
	}

	m_players.clear();

	m_is_running = false;
	Q_EMIT finished();
}

double Engine::getStreamTime()
{
	return Pa_GetStreamTime( m_stream );
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

	m_player_update_timer->start();
	m_is_running = true;
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
	QReadLocker read_lock( engine->m_players_lock );

	float* out = ( float* )outputBuffer;
	std::fill( out, out + ( framesPerBuffer * engine->m_channel_count ), 0.0f );

	QMutableListIterator< Player::Ptr > itr( engine->m_players );
	while ( itr.hasNext() )
	{
		Player::Ptr player = itr.next();
		player->addData( out, framesPerBuffer, engine->m_channel_count );

		if ( !player->isPlaying() )
		{
			read_lock.unlock();
			{
				QWriteLocker write_lock( engine->m_players_lock );

				player->updateTime();
				itr.remove();
			}
			read_lock.relock();
		}
	}

	if ( engine->m_players.isEmpty() )
	{
		engine->m_is_running = false;
		Q_EMIT engine->finished();

		return paComplete;
	}

	return paContinue;
}

} /* namespace audio */
} /* namespace noises */

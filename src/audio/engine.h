#pragma once

#include <QObject>
#include <QMutex>
#include <QList>

#include <portaudio.h>

#include "player.h"

namespace noises
{
namespace audio
{
	class Engine: public QObject
	{
		Q_OBJECT
	public:
		Engine( QObject* parent = nullptr );

		~Engine();

		// Add a player to the engine for it to play

		void registerPlayer( Player::Ptr player );

		void registerPlayers( QList< Player::Ptr >& players );

		// Stop all players

		void stop();

		bool isRunning()
		{
			return is_running;
		}

	Q_SIGNALS:
		void finished();

	public slots:
		void start();

	private:
		static int audioCallback(
			const void* inputBuffer,
			void* outputBuffer,
			unsigned long framesPerBuffer,
			const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags,
			void* userData );

		static void finishedCallback( void* userData );

	private:
		typedef QList< Player::Ptr > PlayerList;

		static PaStream* m_stream;
		static int m_channel_count;

		bool is_running = false;

		QMutex m_players_lock;
		PlayerList m_players;
	};

} /* namespace audio */
} /* namespace noises */

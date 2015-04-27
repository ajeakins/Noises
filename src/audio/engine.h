#pragma once

#include <QObject>
#include <QList>

#include <portaudio.h>

#include "player.h"

class QTimer;
class QReadWriteLock;

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
			return m_is_running;
		}

		// returns the stream time in seconds
		// currently does not work, seems to be
		// a bug in port audio
		double getStreamTime();

	Q_SIGNALS:
		void finished();

	public slots:
		void start();

	private slots:
		void updatePlayerTimes();

	private:
		static int audioCallback(
			const void* inputBuffer,
			void* outputBuffer,
			unsigned long framesPerBuffer,
			const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags,
			void* userData );

	private:
		typedef QList< Player::Ptr > PlayerList;

		PaStream* m_stream;
		int m_channel_count;

		bool m_is_running = false;

		QReadWriteLock* m_players_lock;
		PlayerList m_players;

		// timer for the players to update their owners
		// about their status
		QTimer* m_player_update_timer;
	};

} /* namespace audio */
} /* namespace noises */

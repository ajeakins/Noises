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
		Engine( QObject* parent = 0 );

		~Engine();

		// Add a player to the engine for it to play

		void registerPlayer( Player::Ptr player );

		// Stop all players

		void stop();

	Q_SIGNALS:
		void finished();

	public slots:
		void run();

	private:
		void setup();

		static void streamFinished( void* userData );

		static int audioCallback(
			const void *inputBuffer,
			void *outputBuffer,
			unsigned long framesPerBuffer,
			const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags,
			void *userData );

	private:
		typedef QList< Player::Ptr > PlayerList;

		struct PlaybackData
		{
			QMutex m_lock;
			PlayerList m_players;
		};

		static PaStream* stream;

		PlaybackData m_playback_data;

	};

} /* namespace audio */
} /* namespace noises */
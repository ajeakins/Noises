
#pragma once

#include <QObject>
#include <QThread>
#include <QList>

#include <portaudio.h>

#include "player.h"
#include "hub.h"

namespace noises
{
namespace audio
{
	class Manager: public QObject
	{
		Q_OBJECT
	public:
		Manager();

		~Manager();

		void stop();

		// make private, friend player

		void registerPlayer( Player::Ptr player );

		void unregisterPlayer( Player::Ptr player );

	private Q_SLOTS:
		void playerStarted( Player::Ptr player );

	private:
		QList< Player::Ptr > m_players;

		QThread* m_thread;
		Hub* m_hub;

	};

} /* namespace audio */
} /* namespace noises */

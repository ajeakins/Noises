#pragma once

/**
 * Manages and set of Players, players are requested
 * by users who then interact with them via their member
 * methods. The player keeps track of it's parent which is
 * imutable and notifies the Manager when its parent is delted.
 */

#include <QObject>
#include <QList>

#include <portaudio.h>

#include "player.h"

class QThread;

namespace noises
{
namespace audio
{
	class Engine;

	class Manager: public QObject
	{
		Q_OBJECT
	public:
		Manager();

		~Manager();

		Player::Ptr createPlayer( QObject* parent );

		void stop();

	Q_SIGNALS:
		void started();

		void stopped();

	private Q_SLOTS:
		void playerStarted( Player::Ptr player );

		void unregisterPlayer( Player::Ptr player );

	private:
		QList< Player::Ptr > m_players;

		QThread* m_thread = nullptr;
		Engine* m_engine = nullptr;

	};

} /* namespace audio */
} /* namespace noises */

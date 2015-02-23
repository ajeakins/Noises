#pragma once

/**
 * The manager is the hub for the interaction with the audio system.
 *
 * The manages acts a factory for players, players are requested
 * by users who then interact with them via their member methods.
 * The manager listens to signals from these member methods and
 * manages the interaction with the audio engine.
 */

#include <QObject>
#include <QList>

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
		Manager( QObject* parent = nullptr );

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

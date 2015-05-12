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

#include "engine.h"
#include "player/player.h"
#include "player/types.h"

class QThread;

namespace noises
{
namespace audio
{
	class Engine;
	class Manager;

	class ScopedQueueController
	{
	public:
		ScopedQueueController( Manager& manager );

		~ScopedQueueController();

	private:
		Manager& m_manager;
	};

	class Manager: public QObject
	{
		Q_OBJECT
	public:
		Manager( QObject* parent = nullptr );

		~Manager();

		// Time in seconds
		double getStreamTime();

		Player::Ptr createPlayer( QObject* parent, PlayerType type );

		void stop();

		// Cue players so the can be added to the engine in one
		// go, can be called recursively, each queue call must
		// be matched by and unqueue. The best way to do this is
		// to use the ScopedQueueController.

		void queuePlayers();

		void unqueuePlayers();

	Q_SIGNALS:
		void started();

		void stopped();

	public Q_SLOTS:
		// Remove a player from the manager allowing it to be destroyed
		void releasePlayer( Player::Ptr player );

	private Q_SLOTS:
		void playerStarted( Player::Ptr player );

	private:
		void startEngine();

	private:
		QList< Player::Ptr > m_players;

		unsigned int m_queue_players = 0;
		QList< Player::Ptr > m_queued_players;

		Engine m_engine;
	};

} /* namespace audio */
} /* namespace noises */


#include "audio_player.h"
#include "wait_player.h"
#include "manager.h"

namespace noises
{
namespace audio
{

ScopedQueueController::ScopedQueueController( Manager& manager )
:
	m_manager( manager )
{
	m_manager.queuePlayers();
}

ScopedQueueController::~ScopedQueueController()
{
	m_manager.unqueuePlayers();
}

Manager::Manager( QObject* parent )
:
	QObject( parent )
{
	connect(
		&m_engine, &Engine::finished,
		[this](){ Q_EMIT stopped(); } );
}

Manager::~Manager()
{}

double Manager::getStreamTime()
{
	return m_engine.getStreamTime();
}

Player::Ptr Manager::createPlayer( QObject* parent, PlayerType type )
{
	Player::Ptr player;

	switch( type )
	{
	case PlayerType_Audio:
		player = QSharedPointer< Player >( new AudioPlayer( parent ) );
		break;
	case PlayerType_Wait:
		player = Player::Ptr( new WaitPlayer( parent ) );
	}

	connect(
		player.data(), SIGNAL( started( Player::Ptr ) ),
		this, SLOT( playerStarted( Player::Ptr ) ) );

	connect(
		player.data(), SIGNAL( parentDestroyed( Player::Ptr ) ),
		this, SLOT( unregisterPlayer( Player::Ptr ) ) );

	m_players.append( player );

	return player;
}

void Manager::stop()
{
	m_engine.stop();
}

void Manager::queuePlayers()
{
	++m_queue_players;
}

void Manager::unqueuePlayers()
{
	--m_queue_players;
	if ( m_queue_players == 0 && !m_queued_players.isEmpty() )
	{
		m_engine.registerPlayers( m_queued_players );
		m_queued_players.clear();
		startEngine();
	}
}

void Manager::unregisterPlayer( Player::Ptr player )
{
	m_players.removeOne( player );
}

void Manager::playerStarted( Player::Ptr player )
{
	if ( m_queue_players != 0 )
	{
		m_queued_players.append( player );
	}
	else
	{
		m_engine.registerPlayer( player );
		startEngine();
	}
}

void Manager::startEngine()
{
	if ( !m_engine.isRunning() )
	{
		m_engine.start();
		Q_EMIT started();
	}
}

} /* namespace audio */
} /* namespace noises */

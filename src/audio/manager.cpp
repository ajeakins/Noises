
#include "audio_player.h"
#include "manager.h"

namespace noises
{
namespace audio
{

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

Player::Ptr Manager::createPlayer( QObject* parent, PlayerType type )
{
	Player::Ptr player;

	switch( type )
	{
	case PlayerType_Audio:
		player = QSharedPointer< Player >( new AudioPlayer( parent ) );
		break;
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

void Manager::setQueuePlayers( bool yes )
{
	m_queue_players = yes;
	if ( !m_queue_players && !m_queued_players.isEmpty() )
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
	if ( m_queue_players )
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

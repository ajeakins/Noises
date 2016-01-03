
#include "player/audio_player.h"
#include "player/fade_player.h"
#include "player/wait_player.h"
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
{}

Manager::~Manager()
{}

double Manager::getStreamTime()
{
	return m_engine.getStreamTime();
}

Player::Ptr Manager::createPlayer( PlayerType type )
{
	Player::Ptr player;

	switch( type )
	{
	case PlayerType_Audio:
		player = Player::Ptr( new AudioPlayer );
		break;
	case PlayerType_Wait:
		player = Player::Ptr( new WaitPlayer );
		break;
	case PlayerType_Fade:
		player = Player::Ptr( new FadePlayer );
		break;
	}

	connect(
		player.data(), &Player::started,
		this, &Manager::playerStarted);

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

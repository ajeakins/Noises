
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

Player::Ptr Manager::createPlayer( QObject* parent )
{
	Player::Ptr player = Player::ManagerHooks::create( parent );

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

void Manager::unregisterPlayer( Player::Ptr player )
{
	m_players.removeOne( player );
}

void Manager::playerStarted( Player::Ptr player )
{
	m_engine.registerPlayer( player );
	if ( !m_engine.isRunning() )
	{
		m_engine.start();
		Q_EMIT started();
	}
}

} /* namespace audio */
} /* namespace noises */

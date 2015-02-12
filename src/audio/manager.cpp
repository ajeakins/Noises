
#include <iostream>

#include <QThread>

#include "engine.h"
#include "manager.h"

namespace noises
{
namespace audio
{

Manager::Manager()
{
	m_thread = new QThread();
	m_engine = new Engine();

	m_engine->moveToThread( m_thread );

	connect(
		m_thread, &QThread::started,
		m_engine, &Engine::run );

	connect(
		m_engine, &Engine::finished,
		m_thread, &QThread::quit );
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
	m_engine->stop();
}

void Manager::unregisterPlayer( Player::Ptr player )
{
	m_players.removeOne( player );
}

void Manager::playerStarted( Player::Ptr player )
{
	m_engine->registerPlayer( player );

	if ( !m_thread->isRunning() )
	{
		m_thread->start();
	}
}

} /* namespace audio */
} /* namespace noises */

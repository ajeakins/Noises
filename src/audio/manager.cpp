
#include <iostream>

#include "manager.h"

namespace noises
{
namespace audio
{

Manager::Manager()
{
	m_thread = new QThread();
	m_hub = new Hub();

	m_hub->moveToThread( m_thread );

	connect(
		m_thread, SIGNAL( started() ),
		m_hub, SLOT( run() ) );

	connect(
		m_hub, SIGNAL( finished() ),
		m_thread, SLOT( quit() ) );
}

Manager::~Manager()
{}

void Manager::stop()
{
	m_hub->stop();
}

void Manager::registerPlayer( Player::Ptr player )
{
	m_players.append( player );
	connect(
		player.data(), SIGNAL( started( Player::Ptr ) ),
		this, SLOT( playerStarted( Player::Ptr ) ) );
}

void Manager::unregisterPlayer( Player::Ptr player )
{
	m_players.removeOne( player );
}

void Manager::playerStarted( Player::Ptr player )
{
	std::cout << "playerStarted" << std::endl;


	m_hub->registerPlayer( player );

	if ( !m_thread->isRunning() )
	{
		m_thread->start();
	}
}

} /* namespace audio */
} /* namespace noises */

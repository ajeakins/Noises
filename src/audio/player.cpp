
#include <iostream>

#include <sndfile.h>

#include <application.h>

#include "player.h"

namespace noises
{
namespace audio
{

Player::Player()
:
	m_is_playing( false ),
	m_pos( 0 ),
	m_length( 0 )
{}

Player::~Player()
{}

Player::Ptr Player::create()
{
	Ptr player = Ptr( new Player );
	return player;
}

void Player::setFilename( const QString& filename )
{
	std::cout << "Player::setFilename" << std::endl;

	stop();
	m_filename = filename;
	readData();
}

void Player::readData()
{
	SF_INFO info;
	info.format = 0;

	const char* filename_char = qPrintable( m_filename );
	SNDFILE* file = sf_open( filename_char, SFM_READ, &info );

	if (!file)
	{
		std::cout << "libsndfile error " << filename_char << std::endl;
		std::cout << sf_strerror(file) << std::endl;

		sf_close( file );
	}

	m_length = info.channels * info.frames;
	m_audio_data = new float[m_length];

	sf_readf_float( file, m_audio_data, info.frames );
	sf_close( file );
}

void Player::start()
{
	if ( m_is_playing )
	{
		return;
	}

	m_is_playing = true;
	Q_EMIT started( sharedFromThis() );
}

void Player::stop()
{
	m_is_playing = false;
	m_pos = 0;
	Q_EMIT stopped();
}


} /* namespace audio */
} /* namespace noises */


#include <assert.h>

#include <iostream>

#include <QTime>

#include <sndfile.h>

#include <application.h>

#include "player.h"

namespace noises
{
namespace audio
{

// Manager Hooks

Player::Ptr Player::ManagerHooks::create( QObject* parent )
{
	assert( parent );

	Ptr player = Ptr( new Player( parent ) );
	return player;
}

// Player

Player::Player( QObject* parent )
:
	QObject(), // don't ask parent to manage lifetime
	m_parent( parent ),
	m_is_playing( false ),
	m_audio_data( 0 ),
	m_pos( 0 ),
	m_length( 0 )
{
	connect(
		parent, SIGNAL( destroyed() ),
		this, SLOT( onParentDestroyed() ) );
}

Player::~Player()
{
	free( m_audio_data );
}

void Player::setFilename( const QString& filename )
{
	if ( filename != m_filename )
	{
		stop();
		m_filename = filename;
		readData();
	}
}

void Player::getDuration( QTime& time ) const
{
	int timeInMSecs = ( m_frames * 1000 ) / m_sample_rate;
	time = time.addMSecs( timeInMSecs );
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
	if ( !m_audio_data )
	{
		m_audio_data = ( float* )malloc( m_length * sizeof( float ) );
	}
	else
	{
		m_audio_data = ( float* )realloc( m_audio_data, m_length * sizeof( float ) );
	}

	m_channels = info.channels;
	m_sample_rate = info.samplerate;
	m_frames = info.frames;

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

void Player::onParentDestroyed()
{
	Q_EMIT parentDestroyed( sharedFromThis() );
}

} /* namespace audio */
} /* namespace noises */

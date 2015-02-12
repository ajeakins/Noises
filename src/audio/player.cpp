
#include <assert.h>

#include <QDebug>
#include <QMessageBox>
#include <QTime>

#include <application.h>
#include <utils/audio.h>

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
	m_parent( parent )
{
	connect(
		parent, &QObject::destroyed,
		[this](){ Q_EMIT parentDestroyed( sharedFromThis() ); } );
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

void Player::setVolume( const widgets::MatrixSettings& settings )
{
	m_outputs = Application::getPreferences().getOutputCount();

	for ( int i = 0; i != m_audio_info.channels; ++i )
	{
		m_volumes[i].resize( m_outputs );

		for ( int j = 0; j != m_outputs; ++j )
		{
			float volume = 0.0f;

			float volume_in_db = settings.getVolume( i, j );
			if ( !settings.isMinimumVolume( volume_in_db ) )
			{
				volume = utils::dbToMultiplier( volume_in_db );
			}

			m_volumes[i][j] = volume;
		}
	}
}

float Player::getVolume( int input, int output ) const
{
	if ( input >= m_audio_info.channels || output >= m_outputs )
	{
		return 0.0f;
	}
	return m_volumes[input][output];
}

void Player::getDuration( QTime& time ) const
{
	if ( !m_audio_data )
	{
		return;
	}

	time = timeFromFrames( m_audio_info.frames );
}

QTime Player::timeFromFrames( int frames ) const
{
	QTime time;

	// calculate in floating point
	float timeInMSecs = ( float )frames;
	timeInMSecs *= 1000.0f;
	timeInMSecs /= m_audio_info.samplerate;

	time = time.addMSecs( (int)timeInMSecs );

	return time;
}

void Player::readData()
{
	// indicate to library to fill in the info
	m_audio_info.format = 0;

	std::string data = m_filename.toStdString();
	SNDFILE* file = sf_open( data.c_str(), SFM_READ, &m_audio_info );

	if (!file)
	{
		QString title = "Unable to Open File";
		QString message = "Unable to open audio file [%1].\n\n %2";
		message = message.arg( m_filename, sf_strerror( file ) );
		QMessageBox::critical( nullptr, title, message );
		return;
	}

	m_length = m_audio_info.channels * m_audio_info.frames;
	if ( !m_audio_data )
	{
		m_audio_data = ( float* )malloc( m_length * sizeof( float ) );
		if( !m_audio_data )
		{
			QString title = "Unable to Open File";
			QString message = "Unable to open audio file, memory allocation failed.";
			QMessageBox::critical( nullptr, title, message );
			return;
		}
	}
	else
	{
		float* new_data = ( float* )realloc( m_audio_data, m_length * sizeof( float ) );
		if ( !new_data )
		{
			free( m_audio_data );
			m_audio_data = nullptr;

			QString title = "Unable to Open File";
			QString message = "Unable to open audio file, memory allocation failed.";
			QMessageBox::critical( nullptr, title, message );
			return;
		}
		m_audio_data = new_data;
	}

	sf_readf_float( file, m_audio_data, m_audio_info.frames );
	sf_close( file );

	m_volumes.resize( m_audio_info.channels );
}

void Player::start()
{
	if ( m_is_playing )
	{
		return;
	}

	if ( !m_audio_data )
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

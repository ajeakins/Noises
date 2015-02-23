
#include <assert.h>

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
	m_parent( parent ),
	m_outputs( Application::getPreferences().getOutputCount() )
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

QTime Player::getDuration() const
{
	if ( !m_audio_data )
	{
		return QTime();
	}

	return timeFromFrames( m_audio_info.frames );
}

QTime Player::timeFromFrames( int frames ) const
{
	QTime time(0, 0, 0);

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

	// emit time updated every 100ms
	m_signal_interval = m_audio_info.samplerate / ( 100 * m_audio_info.channels );

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

	// TODO: get rid of this...
	m_volumes.resize( m_audio_info.channels );
	for ( int i = 0; i != m_audio_info.channels; ++i )
	{
		m_volumes[i].resize( m_outputs );
	}
}

/**
 * Add the players data for it's current position to
 * the audio buffer. The buffer is assumed to be managed
 * by the engine so the player.
 */

void Player::addData(
	float* audio,
	int frames,
	int channels )
{
	if ( !m_is_playing )
	{
		return;
	}

	for( int frame = 0; frame != frames; ++frame )
	{
		for( int output = 0; output != channels; ++output )
		{
			for( int input = 0; input != m_audio_info.channels; ++input )
			{
				audio[output] += getVolume( input, output ) * m_audio_data[m_pos + input];
			}
		}

		// increment to next frame
		m_pos += m_audio_info.channels;
		audio += channels;

		// send time update
		if ( m_pos > m_last_pos + m_signal_interval )
		{
			// recievers should be in another thread so this should not block
			Q_EMIT timeUpdated( timeFromFrames( m_pos / m_audio_info.channels ) );
			m_last_pos = m_pos;
		}

		// reached the end of the track
		if ( m_pos == m_length )
		{
			Q_EMIT timeUpdated( QTime( 0, 0, 0 ) );

			m_is_playing = false;
			m_pos = m_last_pos = 0;
		}
	}
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

void Player::pause()
{
	m_is_playing = false;
	Q_EMIT stopped();
}

void Player::stop()
{
	m_is_playing = false;
	m_pos = m_last_pos = 0;
	Q_EMIT timeUpdated( timeFromFrames( 0 ) );
	Q_EMIT stopped();
}

} /* namespace audio */
} /* namespace noises */

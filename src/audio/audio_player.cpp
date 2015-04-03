
#include <assert.h>

#include <QMessageBox>
#include <QTime>

#include <app/application.h>
#include <utils/audio.h>

#include "audio_player.h"

namespace noises
{
namespace audio
{

// AudioPlayer

AudioPlayer::AudioPlayer( QObject* parent )
:
	Player( parent ),
	m_outputs( Application::getPreferences().getOutputCount() )
{}

AudioPlayer::~AudioPlayer()
{
	free( m_audio_data );
	m_audio_data = nullptr;
}

void AudioPlayer::setFilename( const QString& filename )
{
	if ( filename != m_filename )
	{
		stop();
		m_filename = filename;
		readData();
	}
}

void AudioPlayer::setVolume( const widgets::MatrixSettings& settings )
{
	if ( !m_audio_data )
	{
		return;
	}

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

float AudioPlayer::getVolume( int input, int output ) const
{
	if (
		!m_audio_data ||
		input >= m_audio_info.channels ||
		output >= m_outputs )
	{
		return 0.0f;
	}
	return m_volumes[input][output];
}

QTime AudioPlayer::getDuration() const
{
	if ( !m_audio_data )
	{
		return QTime();
	}

	return timeFromFrames( m_audio_info.frames );
}

QTime AudioPlayer::timeFromFrames( int frames ) const
{
	QTime time(0, 0, 0);

	if ( !m_audio_data )
	{
		return time;
	}

	// calculate in floating point
	float timeInMSecs = ( float )frames;
	timeInMSecs *= 1000.0f;
	timeInMSecs /= m_audio_info.samplerate;

	time = time.addMSecs( ( int )timeInMSecs );

	return time;
}

void AudioPlayer::readData()
{
	// indicate to library to fill in the info
	m_audio_info.format = 0;

	std::string data = m_filename.toStdString();
	SNDFILE* file = sf_open( data.c_str(), SFM_READ, &m_audio_info );

	if ( !file )
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

	// emit time updated every 100ms
	m_signal_interval = m_audio_info.samplerate / ( 100 * m_audio_info.channels );

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

void AudioPlayer::addData(
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

void AudioPlayer::start()
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

void AudioPlayer::pause()
{
	m_is_playing = false;
	Q_EMIT stopped();
}

void AudioPlayer::stop()
{
	m_is_playing = false;
	m_pos = m_last_pos = 0;
	Q_EMIT timeUpdated( QTime( 0, 0, 0 ) );
	Q_EMIT stopped();
}

} /* namespace audio */
} /* namespace noises */

#include <assert.h>

#include <QTime>

#include <sndfile.h>

#include <app/application.h>
#include <utils/audio.h>
#include <utils/math.h>

#include <audio/volume_matrix.h>

#include "audio_player.h"

namespace noises
{
namespace audio
{

// AudioPlayer

AudioPlayer::AudioPlayer()
:
	Player(),
	m_audio_info( new SF_INFO )
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

void AudioPlayer::setVolume( const VolumeMatrix& settings )
{
	m_volumes = settings;
	resetVolumes();
}

void AudioPlayer::resetVolumes()
{
	m_current_volumes.resize( m_volumes.getInputs() );
	for ( unsigned int in = 0; in != m_volumes.getInputs(); ++in )
	{
		m_current_volumes[in].resize( m_volumes.getOutputs() );
		for ( unsigned int out = 0; out != m_volumes.getOutputs(); ++out )
		{
			m_current_volumes[in][out] = utils::dbToMultiplier( m_volumes.getVolume( in, out ) );;
		}
	}
}

void AudioPlayer::updateCurrentVolumes()
{
	// No update needed
	if ( m_fades.isEmpty() )
	{
		return;
	}

	int frames = timeInFrames();

	for ( unsigned int in = 0; in != m_volumes.getInputs(); ++in )
	{
		for ( unsigned int out = 0; out != m_volumes.getOutputs(); ++out )
		{
			float cumulative_volume = 1.0f;
			bool any_fade_active = false;

			// add the contribution of all currently running fades
			QMutableListIterator< FadeEntry > itr( m_fades );
			while ( itr.hasNext() )
			{
				FadeEntry& fade_entry = itr.next();

				int local_time = frames - fade_entry.start_frame;
				// fade hasn't started yet
				if ( local_time < 0 )
				{
					continue;
				}
				else
				{
					float from_volume = fade_entry.m_start_volumes[in][out];
					float to_volume = utils::dbToMultiplier( fade_entry.fade->getTargetLevels().getVolume( in, out ) );
					float fraction = ( float )local_time / ( float )fade_entry.fade->getFadeTimeInFrames();
					cumulative_volume *= utils::lerp( from_volume, to_volume, fraction );
					any_fade_active = true;
				}

				if ( local_time >= fade_entry.fade->getFadeTimeInFrames() )
				{
					fade_entry.fade->setPosition( local_time );
					itr.remove();
				}
			}

			if ( any_fade_active )
			{
				m_current_volumes[in][out] = cumulative_volume;
			}
			else
			{
				// nothing to do
				return;
			}
		}
	}
}

QTime AudioPlayer::getDuration() const
{
	if ( !m_audio_data )
	{
		return QTime();
	}

	return timeFromFrames( m_audio_info->frames );
}

void AudioPlayer::addFade( FadePlayer::Ptr fade_player )
{
	FadeEntry entry = { timeInFrames(), fade_player, m_current_volumes };
	m_fades.append( entry );
}

int AudioPlayer::timeInFrames() const
{
	return m_pos / m_audio_info->channels;
}

QTime AudioPlayer::timeFromFrames( int frames ) const
{
	QTime time(0, 0, 0);

	if ( !m_audio_data || frames == 0 )
	{
		return time;
	}

	// calculate in floating point
	float timeInMSecs = ( float )frames;
	timeInMSecs *= 1000.0f;
	timeInMSecs /= m_audio_info->samplerate;

	time = time.addMSecs( ( int )timeInMSecs );

	return time;
}

void AudioPlayer::readData()
{
	// indicate to library to fill in the info
	m_audio_info->format = 0;

	std::string data = m_filename.toStdString();
	SNDFILE* file = sf_open( data.c_str(), SFM_READ, m_audio_info.data() );

	if ( !file )
	{
		onError(
			"Unable to Open File",
			QString("Unable to open audio file [%1].\n\n %2").arg( m_filename, sf_strerror( file ) ) );
		return;
	}

	m_length = m_audio_info->channels * m_audio_info->frames;
	if ( !m_audio_data )
	{
		m_audio_data = ( float* )malloc( m_length * sizeof( float ) );
		if( !m_audio_data )
		{
			onError(
				"Unable to Open File",
				"Unable to open audio file, memory allocation failed." );
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

			onError(
				"Unable to Open File",
				"Unable to open audio file, memory allocation failed." );
			return;
		}
		m_audio_data = new_data;
	}

	sf_readf_float( file, m_audio_data, m_audio_info->frames );
	sf_close( file );

	// TODO: get rid of this... maybe
	m_volumes.setInputs( m_audio_info->channels );
	m_volumes.setOutputs( Application::getPreferences().getOutputCount() );
}

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
		updateCurrentVolumes();

		for( int output = 0; output != channels; ++output )
		{
			for( int input = 0; input != m_audio_info->channels; ++input )
			{
				audio[output] +=  m_current_volumes[input][output] * m_audio_data[m_pos + input];
			}
		}

		// increment to next frame
		m_pos += m_audio_info->channels;
		audio += channels;

		// reached the end of the track
		if ( m_pos == m_length )
		{
			stop();
			return;
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
	m_pos = 0;
	resetVolumes();
	Q_EMIT stopped();
}

void AudioPlayer::updateTime() const
{
	int frames = timeInFrames();

	// update our time
	QTime time = timeFromFrames( frames );
	Q_EMIT timeUpdated( time );

	// stimulate fades to update theirs
	for ( Fades::const_iterator itr = m_fades.begin(); itr != m_fades.end(); ++itr )
	{
		itr->fade->setPosition( frames - itr->start_frame );
		itr->fade->updateTime();
	}
}

} /* namespace audio */
} /* namespace noises */

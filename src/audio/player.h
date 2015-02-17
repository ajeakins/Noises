
#pragma once

#include <QDebug>
#include <QObject>
#include <QSharedPointer>
#include <QTime>
#include <QEnableSharedFromThis>

#include <portaudio.h>
#include <sndfile.h>

#include <widgets/matrix.h>

namespace noises
{
namespace audio
{
	class Player: public QObject, public QEnableSharedFromThis< Player >
	{
		Q_OBJECT
	public:
		typedef QSharedPointer< Player > Ptr;

		class ManagerHooks
		{
		private:
			friend class Manager;

			static Ptr create( QObject* parent );
		};

		~Player();

		// setup

		void setFilename( const QString& filename );

		void setVolume( const widgets::MatrixSettings& settings );

		QTime getDuration() const;

		// Playback controls

		void start();

		void pause();

		void stop();

		bool isPlaying() const
		{
			return m_is_playing;
		}

	Q_SIGNALS:
		void started( Player::Ptr player );

		void stopped();

		void timeUpdated( QTime time );

		void parentDestroyed( Player::Ptr player );

	private:
		friend class ManagerHooks;

		friend class Engine;

		Player( QObject* parent );

		float getVolume( int input, int output ) const;

		void readData();

		QTime timeFromFrames( int frames ) const;

		inline void addData( float* audio_data, int channels );

	private:
		QObject* m_parent;

		QString m_filename;

		// playback state
		bool m_is_playing = false;
		int m_pos = 0 ;
		int m_last_pos = 0;

		// audio data
		float* m_audio_data = nullptr;

		// audio information
		SF_INFO m_audio_info;
		int m_length;
		int m_signal_interval;

		// volume data
		QVector< QVector< float > > m_volumes;
		// current number of outputs we have data for
		int m_outputs;
	};


	/**
	 * Add the players data for it's current position to
	 * the audio buffer. The buffer is assumed to be managed
	 * by the engine so the player.
	 */

	void Player::addData( float* audio, int channels )
	{
		if ( !m_is_playing )
		{
			return;
		}

		for( int output = 0; output != channels; ++output )
		{
			for( int input = 0; input != m_audio_info.channels; ++input )
			{
				audio[output] += getVolume( input, output ) * m_audio_data[m_pos + input];
			}
		}

		// increment to next frame
		m_pos += m_audio_info.channels;

		// send time update
		if ( m_pos > m_last_pos + m_signal_interval )
		{
			// Recievers should be in another thread so this should not block
			Q_EMIT timeUpdated( timeFromFrames( m_pos / m_audio_info.channels ) );
			m_last_pos = m_pos;
		}

		// reached the end of the track
		if ( m_pos == m_length )
		{
			Q_EMIT timeUpdated( timeFromFrames( m_pos / m_audio_info.channels ) );

			m_is_playing = false;
			m_pos = m_last_pos = 0;
		}
	}

} /* namespace audio */
} /* namespace noises */

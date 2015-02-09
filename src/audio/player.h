
#pragma once

#include <QDebug>
#include <QObject>
#include <QSharedPointer>
#include <QEnableSharedFromThis>

#include <portaudio.h>

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

		void getDuration( QTime& time ) const;

		// Playback controls

		void start();

		void stop();

		bool isPlaying() const
		{
			return m_is_playing;
		}

	Q_SIGNALS:
		void started( Player::Ptr player );

		void stopped();

		void parentDestroyed( Player::Ptr player );

	private Q_SLOTS:
		void onParentDestroyed();

	private:
		friend class ManagerHooks;

		friend class Engine;

		Player( QObject* parent );

		float getVolume( int input, int output );

		void readData();

		inline void addData( float* audio_data, int channels );

	private:
		QObject* m_parent;

		QString m_filename;

		bool m_is_playing;

		float* m_audio_data;
		int m_pos;
		int m_length;
		int m_channels;
		int m_sample_rate;
		int m_frames;

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
			for( int input = 0; input != m_channels; ++input )
			{
				audio[output] += getVolume( input, output ) * m_audio_data[m_pos + input];
			}
		}

		// increment to next frame
		m_pos += m_channels;

		// reached the end of the track
		if ( m_pos == m_length )
		{
			m_is_playing = false;
			m_pos = 0;
		}
	}

} /* namespace audio */
} /* namespace noises */


#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QEnableSharedFromThis>

#include <portaudio.h>

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

		void setVolume( widgets::MatrixSettings& settings );

		void setVolume(
			unsigned int input,
			unsigned int output,
			float volume_in_db );

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

		void readData();

		inline float data();

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
	};


	/**
	 * Add the players data for it's current position to
	 * the audio buffer. The buffer is assumed to be managed
	 * by the engine so the player.
	 *
	 * This could probably be optimised alot but
	 * just getting it working for the moment!
	 */

	void Player::addData( float* audio, int channels )
	{
		if ( !m_is_playing )
		{
			return;
		}

		// always make sure pos is incremented by the number
		// of channels we have
		for( int i = 0; i != m_channels; ++i )
		{
			if ( i != channels )
			{
				audio[i] += m_audio_data[m_pos];
			}
			++m_pos;
		}

		// reached the end of the track
		if ( m_pos == m_length )
		{
			m_is_playing = false;
			m_pos = 0;
		}
	}

} /* namespace audio */
} /* namespace noises */

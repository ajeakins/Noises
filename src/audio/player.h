
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

	float Player::data()
	{
		float res = 0.0f;

		if ( m_pos < m_length )
		{
			res = m_audio_data[m_pos];
			++m_pos;
		}
		else
		{
			m_is_playing = false;
		};

		return res;
	}

} /* namespace audio */
} /* namespace noises */


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
		// Players exist with managed lifetime

		typedef QSharedPointer< Player > Ptr;

		static Ptr create();

		~Player();

		// setup

		void setFilename( const QString& filename );

		// Playback controls

		void start();

		void stop();

		bool isPlaying()
		{
			return m_is_playing;
		}

		// TODO: make private/friend hub

		float data()
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

	Q_SIGNALS:
		void started( Player::Ptr player );

		void stopped();

	private:
		Player();

		void readData();

	private:
		QString m_filename;

		bool m_is_playing;

		float* m_audio_data;
		int m_pos;
		int m_length;
		int m_channels;
	};

} /* namespace audio */
} /* namespace noises */

#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QTime>
#include <QEnableSharedFromThis>

namespace noises
{
namespace audio
{
	class Player: public QObject, public QEnableSharedFromThis< Player >
	{
		Q_OBJECT
	public:
		typedef QSharedPointer< Player > Ptr;

		virtual ~Player();

		// Playback controls

		virtual void start() = 0;

		virtual void stop() = 0;

		bool isPlaying() const
		{
			return m_is_playing;
		}

		virtual void updateTime() = 0;

	Q_SIGNALS:
		void started( Player::Ptr player );

		void stopped();

		void timeUpdated( QTime time );

		void parentDestroyed( Player::Ptr player );

	protected:
		friend class Manager;
		friend class Engine;

		Player( QObject* parent );

		virtual void addData(
			float* audio_data,
			int frames,
			int channels ) = 0;

		void emitTimeSignal( const QTime& time )
		{
			Q_EMIT timeUpdated( time );
		}

	protected:
		// playback state
		bool m_is_playing = false;

	private:
		QObject* m_parent;
	};

} /* namespace audio */
} /* namespace noises */

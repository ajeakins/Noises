#pragma once

#include "player.h"

namespace noises
{
namespace audio
{
	class WaitPlayer: public Player
	{
		Q_OBJECT
	public:
		typedef QSharedPointer< WaitPlayer > Ptr;

		~WaitPlayer();

		// Playback controls

		void start();

		void stop();

		void updateTime();

		void setDuration( const QTime& time )
		{
			m_duration = time;
		}

	private:
		friend class Manager;
		friend class Engine;

		WaitPlayer( QObject* parent );

		void addData(
			float* audio_data,
			int frames,
			int channels );

	private:
		QTime m_duration;

		int m_pos;
		int m_length;
	};

} /* namespace audio */
} /* namespace noises */

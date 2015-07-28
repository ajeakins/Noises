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

		~WaitPlayer() override;

		// Playback controls

		void start() override;

		void stop() override;

		void updateTime() const override;

		void setDuration( const QTime& time )
		{
			m_duration = time;
		}

	Q_SIGNALS:
		void waitDone();

	private:
		friend class Manager;
		friend class Engine;

		WaitPlayer();

		void addData(
			float* audio_data,
			int frames,
			int channels ) override;

	private:
		QTime m_duration;

		int m_pos;
		int m_length;
	};

} /* namespace audio */
} /* namespace noises */

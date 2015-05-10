#pragma once

#include "player.h"

namespace noises
{
namespace audio
{
	class FadePlayer: public Player
	{
		Q_OBJECT
	public:
		typedef QSharedPointer< FadePlayer > Ptr;

		~FadePlayer() override;

		// Playback controls

		void start() override;

		void stop() override;

		void updateTime() const override;

	Q_SIGNALS:
		void fadeDone();

	private:
		friend class Manager;
		friend class Engine;

		FadePlayer( QObject* parent );

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

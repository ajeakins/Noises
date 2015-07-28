#pragma once

#include <audio/volume_matrix.h>

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

		void setFadeTime( const QTime& time );

		int getFadeTimeInFrames() const
		{
			return m_fade_time_in_frames;
		}

		// Temp interface
		void setPosition( int pos );

		void setTargetLevels( const audio::VolumeMatrix& levels )
		{
			m_target_levels = levels;
		}

		const audio::VolumeMatrix& getTargetLevels() const
		{
			return m_target_levels;
		}

	Q_SIGNALS:
		void fadeDone();

	private:
		friend class Manager;
		friend class Engine;

		FadePlayer();

		void addData(
			float* audio_data,
			int frames,
			int channels ) override;

	private:
		QTime m_fade_time;
		int m_fade_time_in_frames;
		audio::VolumeMatrix m_target_levels;

		int m_pos;
		int m_length;
	};

} /* namespace audio */
} /* namespace noises */

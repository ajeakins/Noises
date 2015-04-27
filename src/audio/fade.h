#pragma once

#include <QTime>

#include "volume_matrix.h"

namespace noises
{
namespace audio
{
	class Fade
	{
	public:
		Fade();

		~Fade();

		void setDuration( const QTime& time );

		void setStart( const VolumeMatrix& start );

		void setEnd( const VolumeMatrix& end );

		VolumeMatrix interpolate( const QTime& time );

	private:
		VolumeMatrix m_start, m_end;
		QTime m_duration;
	};

} /* namespace audio */
} /* namespace noises */

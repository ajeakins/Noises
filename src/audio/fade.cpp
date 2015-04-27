
#include <utils/math.h>
#include <utils/time.h>

#include "fade.h"

namespace noises
{
namespace audio
{

Fade::Fade()
{}

Fade::~Fade()
{}

void Fade::setDuration( const QTime& time )
{
	m_duration = time;
}

void Fade::setStart( const VolumeMatrix& start )
{
	m_start = start;
}

void Fade::setEnd( const VolumeMatrix& end )
{
	m_end = end;
}

VolumeMatrix Fade::interpolate( const QTime& time )
{
	assert( m_start.getInputs() == m_end.getInputs() );
	assert( m_start.getOutputs() == m_end.getOutputs() );

	VolumeMatrix result( m_start.getInputs(), m_start.getOutputs() );

	float fraction = utils::fraction( time, m_duration );
	fraction = utils::clamp( 0.0f, 1.0f, fraction );

	for ( unsigned int i = 0; i != result.getInputs(); ++i )
	{
		for ( unsigned int j = 0; j != result.getOutputs(); ++j )
		{
			float volume = utils::lerp(
				m_start.getVolume( i, j ),
				m_end.getVolume( i, j ),
				fraction );
			result.setVolume( i, j, volume );
		}
	}

	return result;
}

} /* namespace audio */
} /* namespace noises */

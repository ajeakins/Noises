
#include <math.h>

#include "audio.h"

namespace noises
{
namespace utils
{

float dbToMultiplier( float db )
{
	return powf(10, db / 20.0f );
}

float multiplerToDb( float multiplier )
{
	return 20.f * log10( multiplier );
}

} /* namspace noises */
} /* namespace utils */

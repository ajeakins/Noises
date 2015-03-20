
#include "audio.h"

namespace noises
{
namespace utils
{

float dbToMultiplier( float db )
{
	return pow10f( db / 20.0f );
}

float multiplerToDb( float multiplier )
{
	return 20.f * log10f( multiplier );
}

} /* namspace noises */
} /* namespace utils */

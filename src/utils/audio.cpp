
#include "audio.h"

namespace noises
{
namespace utils
{

float dbToMultiplier( float db )
{
	return pow10f( db / 10.0f );
}

float multiplerToDb( float multiplier )
{
	return 10 * log10f( multiplier );
}

} /* namspace noises */
} /* namespace utils */

#pragma once

#include <assert.h>

namespace noises
{
namespace utils
{
	template< typename T >
	T clamp( const T& min, const T& max, const T& value )
	{
		if ( value < min )
		{
			return min;
		}
		else if ( value > max )
		{
			return max;
		}
		return value;
	}

	template< typename T >
	T lerp( const T& from, const T& to, float amount )
	{
		assert( amount >= 0.0f && amount <= 1.0f );
		return ( 1.0f - amount ) * from +  amount * to;
	}

} /* namspace noises */
} /* namespace utils */

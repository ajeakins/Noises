#pragma once

#include <QTime>

namespace noises
{
namespace utils
{
	// all pretty nasty...

	int timeToMsecs( const QTime& time )
	{
		int msecs = 0;
		msecs += time.msec();
		msecs += time.second() * 1000;
		msecs += time.minute() * 60 * 1000;
		msecs += time.hour() * 60 * 60 * 1000;
		return msecs;
	}

	QTime subtract( const QTime& lhs, const QTime& rhs )
	{
		QTime time( 0, 0, 0 );
		time = time.addMSecs( timeToMsecs( lhs ) - timeToMsecs( rhs ) );
		return time;
	}

} /* namspace noises */
} /* namespace utils */
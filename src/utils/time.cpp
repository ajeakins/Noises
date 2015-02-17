
#include "time.h"

namespace noises
{
namespace utils
{

static const QString secondTimeFormat = "ss.zzz 's'";
static const QString minuteTimeFormat = "mm'm' ss.zzz 's'";
static const QString hourTimeFormat = "hh'h' mm'm' ss.zzz";

QString timeFormat( const QTime& time )
{
	if ( time.hour() > 0 )
	{
		return hourTimeFormat;
	}
	else if ( time.minute() > 0 )
	{
		return minuteTimeFormat;
	}
	else
	{
		return secondTimeFormat;
	}
}

QString defaultTimeFormat()
{
	return secondTimeFormat;
}

int timeToMsecs( const QTime& time )
{
	int msecs = 0;
	msecs += time.msec();
	msecs += time.second() * 1000;
	msecs += time.minute() * 60 * 1000;
	msecs += time.hour() * 60 * 60 * 1000;
	return msecs;
}

QTime timeFromMsecs( int msecs )
{
	QTime time( 0, 0 , 0 );
	time = time.addMSecs( msecs );
	return time;
}

QTime subtract( const QTime& lhs, const QTime& rhs )
{
	QTime time( 0, 0, 0 );
	time = time.addMSecs( timeToMsecs( lhs ) - timeToMsecs( rhs ) );
	return time;
}

} /* namspace noises */
} /* namespace utils */
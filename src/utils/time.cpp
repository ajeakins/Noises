
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

bool isZero( const QTime& time )
{
	if (
		time.msec() != 0 ||
		time.second() != 0 ||
		time.minute() != 0 ||
		time.hour() != 0
		)
	{
		return false;
	}

	return true;
}

int timeToMsecs( const QTime& time )
{
	int msecs = time.msec();
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

float percentage( const QTime& numerator, const QTime& denominator )
{
	return ( ( float )timeToMsecs( numerator ) ) * 100.0f / ( ( float )timeToMsecs( denominator ) );
}

} /* namspace noises */
} /* namespace utils */

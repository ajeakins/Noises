#pragma once

#include <QTime>

namespace noises
{
namespace utils
{
	// formats

	QString timeFormat( const QTime& time );

	QString defaultTimeFormat();

	bool isZero( const QTime& time );

	// conversions

	int timeToMsecs( const QTime& time );

	QTime timeFromMsecs( int msecs );

	// operations

	QTime subtract( const QTime& lhs, const QTime& rhs );

	float fraction( const QTime& numerator, const QTime& denominator );

	float percentage( const QTime& numerator, const QTime& denominator );

} /* namspace noises */
} /* namespace utils */

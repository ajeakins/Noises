#pragma once

#include <QTime>

namespace noises
{
namespace utils
{
	// formats

	QString timeFormat( const QTime& time );

	QString defaultTimeFormat();

	// conversions

	int timeToMsecs( const QTime& time );

	QTime timeFromMsecs( int msecs );

	// operations

	QTime subtract( const QTime& lhs, const QTime& rhs );

} /* namspace noises */
} /* namespace utils */
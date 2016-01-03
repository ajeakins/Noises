
#include "time_label.h"

namespace noises
{
namespace widgets
{

void TimeLabel::setPrefix( const QString& prefix )
{
	m_prefix = prefix;
	updateTime();
}

void TimeLabel::setTime( const QTime& time )
{
	m_time = time;
	updateTime();
}

void TimeLabel::setDisplayFormat( const QString& format )
{
	m_display_format = format;
	updateTime();
}

void TimeLabel::updateTime()
{
	setText( m_prefix + m_time.toString( m_display_format ) );
}

}
}
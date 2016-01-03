#pragma once

#include <QLabel>
#include <QTime>

namespace noises
{
namespace widgets
{
	class TimeLabel : public QLabel
	{
		Q_OBJECT
	public:
		TimeLabel( QWidget * parent = nullptr, Qt::WindowFlags f = nullptr )
		: QLabel( parent, f )
		{}

		TimeLabel( const QString& text, QWidget * parent = nullptr, Qt::WindowFlags f = nullptr )
		: QLabel( text, parent, f )
		{}

		void setPrefix( const QString& prefix );

		void setTime( const QTime& time );

		void setDisplayFormat( const QString& format);

	private:
		void updateTime();

	private:
		QTime 		m_time;
		QString 	m_display_format;
		QString 	m_prefix;
	};
}
}
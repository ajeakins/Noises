#pragma once

#include <QWidget>

class QDial;
class QDoubleSpinBox;

namespace noises
{
namespace widgets
{
	class VolumeDial: public QWidget
	{
		Q_OBJECT
	public:
		VolumeDial( QWidget* parent = 0 );

		VolumeDial( const QString& text, QWidget* parent = 0 );

		~VolumeDial();

		static float getMinimumVolume();

		float getVolume() const;

		void setVolume( float value );

	Q_SIGNALS:
		void valueChanged( float value );

	protected:
		void mousePressEvent( QMouseEvent* event );

		virtual void paintEvent( QPaintEvent* event );

	private Q_SLOTS:
		void valueSetDial( int value );
		void valueSetSpinBox( double value );

	private:
		void createWidgets();

	private:
		QString m_text;

		QDial* m_dial;
		QDoubleSpinBox* m_spin_box;

	};

} /* namespace widgets */
} /* namespace noises */

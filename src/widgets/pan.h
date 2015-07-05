#pragma once

#include <QWidget>

class QSlider;
class QSpinBox;

namespace noises
{
namespace widgets
{
	class Pan: public QWidget
	{
		Q_OBJECT
	public:
		Pan( QWidget* parent = nullptr );

		int getValue() const;

		void setValue( int value );

	Q_SIGNALS:
		void valueChanged( int value );

	private:
		void createWidgets();

	private Q_SLOTS:
		void valueSetSlider( int value );

		void valueSetSpinBox( int value );

	private:
		QSlider* m_slider;
		QSpinBox* m_spin_box;
	};

} /* namespace widgets */
} /* namespace noises */

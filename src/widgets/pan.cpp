
#include <QLabel>
#include <QLayout>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

#include "pan.h"

namespace noises
{
namespace widgets
{

Pan::Pan( QWidget* parent )
:
	QWidget( parent )
{
	createWidgets();
}

int Pan::getValue() const
{
	return m_spin_box->value();
}

void Pan::setValue( int value )
{
	m_spin_box->blockSignals( true );
	m_spin_box->setValue( value );
	m_spin_box->blockSignals( false );

	m_slider->blockSignals( true );
	m_slider->setValue( value );
	m_slider->blockSignals( false );
}

void Pan::createWidgets()
{
	m_slider = new QSlider(  Qt::Horizontal, this );
	m_slider->setTickPosition( QSlider::TicksBelow );
	m_slider->setTickInterval( 20 );
	m_slider->setMinimum( -100 );
	m_slider->setMaximum( 100 );

	connect(
		m_slider, SIGNAL( valueChanged( int ) ),
		this, SLOT( valueSetSlider( int ) ) );

	QLabel* left = new QLabel( "L", this );
	QLabel* right = new QLabel( "R", this );

	m_spin_box = new QSpinBox( this );
	m_spin_box->setMinimum( -100 );
	m_spin_box->setMaximum( 100 );

	connect(
		m_spin_box, SIGNAL( valueChanged( int ) ),
		this, SLOT( valueSetSpinBox( int ) ) );

	QHBoxLayout* pan_layout = new QHBoxLayout;
	pan_layout->setContentsMargins( 0, 0, 0, 0 );
	pan_layout->addWidget( left );
	pan_layout->addWidget( m_slider );
	pan_layout->addWidget( right );
	pan_layout->addWidget( m_spin_box );
	setLayout( pan_layout );
}

void Pan::valueSetSlider( int value )
{
	m_spin_box->blockSignals( true );
	m_spin_box->setValue( value );
	m_spin_box->blockSignals( false );

	Q_EMIT valueChanged( value );
}

void Pan::valueSetSpinBox( int value )
{
	m_slider->blockSignals( true );
	m_slider->setValue( value );
	m_slider->blockSignals( false );

	Q_EMIT valueChanged( value );
}

} /* namespace widgets */
} /* namespace noises */

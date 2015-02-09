
#include <limits>
#include <iostream>

#include <QApplication>
#include <QDial>
#include <QLayout>
#include <QDoubleSpinBox>
#include <QMouseEvent>
#include <QPainter>

#include "volume_dial.h"

#define MINIMUM_DB -30
#define MAXIMUM_DB 6
#define STEP 0.1f

namespace noises
{
namespace widgets
{

VolumeDial::VolumeDial( QWidget* parent )
:
	QWidget( parent )
{
	// default to being as small as we can
	setSizePolicy(
		QSizePolicy(
			QSizePolicy::Minimum,
			QSizePolicy::Minimum ) );
	resize( QSize( 0, 0 ) );

	createWidgets();
}

VolumeDial::~VolumeDial()
{}

float VolumeDial::getMinimumVolume()
{
	return ( float )MINIMUM_DB;
}

float VolumeDial::getVolume() const
{
	return m_spin_box->value();
}

void VolumeDial::setVolume( float value_in_db )
{
	int value = ( int )( value_in_db / STEP );

	// TODO: we might be able to design these out...

	m_spin_box->blockSignals( true );
	m_spin_box->setValue( value_in_db );
	m_spin_box->blockSignals( false );

	m_dial->blockSignals( true );
	m_dial->setValue( value );
	m_dial->blockSignals( false );
}

void VolumeDial::paintEvent( QPaintEvent* event )
{
	QWidget::paintEvent( event );
}

void VolumeDial::mousePressEvent( QMouseEvent* event )
{
	if (
		event->type() == QMouseEvent::MouseButtonPress &&
		event->button() == Qt::LeftButton &&
		event->modifiers() & Qt::ControlModifier )
	{
		m_dial->setValue( 0 );
		event->accept();
	}

	QWidget::mousePressEvent( event );
}

void VolumeDial::valueSetDial( int value )
{
	double value_in_db = ( double )value * STEP;

	// TODO: we might be able to design these out...
	m_spin_box->blockSignals( true );
	m_spin_box->setValue( value_in_db );
	m_spin_box->blockSignals( false );

	Q_EMIT valueChanged( value_in_db );
}

void VolumeDial::valueSetSpinBox( double value_in_db )
{
	int value = ( int )( value_in_db / STEP );

	m_dial->blockSignals( true );
	m_dial->setValue( value );
	m_dial->blockSignals( false );

	Q_EMIT valueChanged( value_in_db );
}

void VolumeDial::createWidgets()
{
	m_dial = new QDial( this );

	int step_multiplier = 1.0f / STEP;

	m_dial->setMinimum( step_multiplier * MINIMUM_DB );
	m_dial->setMaximum( step_multiplier * MAXIMUM_DB );
	m_dial->setValue( m_dial->minimum() );
	m_dial->setPageStep( 100 );
	m_dial->setSingleStep( 10 );
	m_dial->setNotchesVisible( true );
	// this is ugly but they won't go small...
	m_dial->setFixedSize( 50, 50 );

	connect(
		m_dial, SIGNAL( valueChanged( int ) ),
		this, SLOT( valueSetDial( int ) ) );

	m_spin_box = new QDoubleSpinBox( this );
	m_spin_box->setMinimum( MINIMUM_DB );
	m_spin_box->setMaximum( MAXIMUM_DB );
	// TODO: extract from define somehow?
	m_spin_box->setDecimals( 1 );
	m_spin_box->setSuffix( "dB" );
	m_spin_box->setValue( MINIMUM_DB );
	m_spin_box->setSpecialValueText( "-inf" );

	connect(
		m_spin_box, SIGNAL( valueChanged( double ) ),
		this, SLOT( valueSetSpinBox( double ) ) );

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins( 0, 0, 0, 0 );

	layout->addWidget( m_dial );
	layout->addWidget( m_spin_box );

	setLayout( layout );
}

} /* namespace widgets */
} /* namespace noises */

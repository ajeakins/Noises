
#include <QApplication>
#include <QDataWidgetMapper>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QStyle>

#include "wait_cue_dialog.h"

namespace noises
{

WaitCueDialog::WaitCueDialog(
	WaitCueModelItem* cue,
	QDataWidgetMapper* mapper,
	QWidget* parent )
:
	CueDialog( mapper, parent ),
	m_cue( cue )
{
	setWindowTitle( "Edit Control Cue" );

	createCueWidgets();

	// read the settings and write into widgets
	readSettings();

	setSizePolicy(
		QSizePolicy(
			QSizePolicy::Minimum,
			QSizePolicy::Minimum ) );
	resize( 0, 0 );
}

WaitCueDialog::~WaitCueDialog()
{}

void WaitCueDialog::accept()
{
	writeSettings();
	CueDialog::accept();
}

void WaitCueDialog::writeSettings()
{
}

void WaitCueDialog::readSettings()
{
}

void WaitCueDialog::createCueWidgets()
{
	createWidgets();

	QLabel* wait_time_label = new QLabel( "Wait time:", this );

	m_wait_time = new QDoubleSpinBox( this );
	m_wait_time->setSuffix( "s" );

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins( 0, 0, 0, 0 );

	layout->addWidget( wait_time_label );
	layout->addWidget( m_wait_time );

	QGroupBox* times_group_box = new QGroupBox( "Times", this );
	times_group_box->setLayout( layout );

	m_layout->addWidget( times_group_box );
}

} /* namespace noises */

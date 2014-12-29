
#include <QApplication>
#include <QDataWidgetMapper>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QStyle>

#include "control_cue_dialog.h"

namespace noises
{

ControlCueDialog::ControlCueDialog(
	ControlCueModelItem* cue,
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

ControlCueDialog::~ControlCueDialog()
{}

void ControlCueDialog::accept()
{
	writeSettings();
	CueDialog::accept();
}

void ControlCueDialog::writeSettings()
{
}

void ControlCueDialog::readSettings()
{
}

void ControlCueDialog::createCueWidgets()
{
	createWidgets();
}

} /* namespace noises */
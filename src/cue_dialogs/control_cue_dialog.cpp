
#include "control_cue_dialog.h"

namespace noises
{

ControlCueDialog::ControlCueDialog(
	ControlCueModelItem* cue,
	QDataWidgetMapper* mapper,
	bool create,
	QWidget* parent )
:
	CueDialog( mapper, create, parent ),
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

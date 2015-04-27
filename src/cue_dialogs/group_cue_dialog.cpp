
#include <cue/group_cue_model_item.h>

#include "group_cue_dialog.h"

namespace noises
{

GroupCueDialog::GroupCueDialog(
	GroupCueModelItem* cue,
	QDataWidgetMapper* mapper,
	bool create,
	QWidget* parent )
:
	CueDialog( mapper, create, parent ),
	m_cue( cue )
{
	setWindowTitle( "Edit Group Cue" );

	createCueWidgets();

	// read the settings and write into widgets
	readSettings();

	setSizePolicy(
		QSizePolicy(
			QSizePolicy::Minimum,
			QSizePolicy::Minimum ) );
	resize( 0, 0 );
}

GroupCueDialog::~GroupCueDialog()
{}

void GroupCueDialog::accept()
{
	writeSettings();
	CueDialog::accept();
}

void GroupCueDialog::writeSettings()
{
}

void GroupCueDialog::readSettings()
{
}

void GroupCueDialog::createCueWidgets()
{
	createWidgets();
}

} /* namespace noises */

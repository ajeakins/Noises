
#include <QApplication>
#include <QDataWidgetMapper>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QStyle>

#include "group_cue_dialog.h"

namespace noises
{

GroupCueDialog::GroupCueDialog(
	GroupCueModelItem* cue,
	QDataWidgetMapper* mapper,
	QWidget* parent )
:
	CueDialog( mapper, parent ),
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
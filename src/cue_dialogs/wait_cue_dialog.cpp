
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QTimeEdit>

#include <utils/time.h>

#include <cue/wait_cue_model_item.h>

#include "detail.h"
#include "wait_cue_dialog.h"

namespace noises
{

WaitCueDialog::WaitCueDialog(
	WaitCueModelItem* cue,
	QDataWidgetMapper* mapper,
	bool create,
	QWidget* parent )
:
	CueDialog( mapper, create, parent ),
	m_cue( cue )
{
	setWindowTitle( "Edit Wait Cue" );

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

bool WaitCueDialog::writeSettings() const
{
	bool something_changed = false;

	WaitCueSettings& settings = m_cue->getSettings();

	SET_VALUE( settings.wait_time, m_wait_time->time(), something_changed )

	m_cue->updatePlayer();

	return something_changed;
}

void WaitCueDialog::readSettings()
{
	const WaitCueSettings& settings = m_cue->getSettings();
	m_wait_time->setTime( settings.wait_time );
}

void WaitCueDialog::createCueWidgets()
{
	createWidgets();

	QLabel* wait_time_label = new QLabel( "Wait time:", this );

	m_wait_time = new QTimeEdit( this );
	m_wait_time->setDisplayFormat( utils::defaultTimeFormat() );

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins( 0, 0, 0, 0 );

	layout->addWidget( wait_time_label );
	layout->addWidget( m_wait_time );

	QGroupBox* times_group_box = new QGroupBox( "Times", this );
	times_group_box->setLayout( layout );

	m_layout->addWidget( times_group_box );
}

} /* namespace noises */

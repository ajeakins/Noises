
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>

#include <cues/cue_model_item.h>
#include <cues/control_cue_model_item.h>
#include <cue_widget/types.h>

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
	ControlCueSettings& settings = m_cue->getSettings();

	settings.cue_action = ( ControlAction )( m_action->itemData( m_action->currentIndex() ).toInt() );
	settings.target_cue_uuid = m_target_cue->itemData( m_target_cue->currentIndex() ).toString();
}

void ControlCueDialog::readSettings()
{
	const ControlCueSettings& settings = m_cue->getSettings();

	int index = m_action->findData( settings.cue_action );
	if ( index >= 0 )
	{
		m_action->setCurrentIndex( index );
	}

	index = m_target_cue->findData( settings.target_cue_uuid );
	if ( index >= 0 )
	{
		m_target_cue->setCurrentIndex( index );
	}
}

void ControlCueDialog::createCueWidgets()
{
	createWidgets();

	QLabel* target_cue_label = new QLabel( "Target Cue:", this );
	m_target_cue = new QComboBox( this );
	populateTargetCues();

	QLabel* action_label = new QLabel( "Action", this );
	m_action = new QComboBox( this );

	for ( ControlAction action = ( ControlAction )0; action != ControlAction_ITEM_COUNT; ++action )
	{
		m_action->addItem( actionToString( action ), action );
	}

	QGridLayout* layout = new QGridLayout;
	layout->setContentsMargins( 0, 0, 0, 0 );

	layout->addWidget( target_cue_label, 0, 0 );
	layout->addWidget( m_target_cue, 0, 1 );

	layout->addWidget( action_label, 1, 0 );
	layout->addWidget( m_action, 1, 1 );

	QGroupBox* group_box = new QGroupBox( "Control Settings", this );
	group_box->setLayout( layout );

	m_layout->addWidget( group_box );
}

namespace
{
	QString getName( CueModelItem* item )
	{
		QString name = item->data( Column_Cue ).toString();
		name += " - ";
		name += item->data( Column_Description ).toString();
		return name;
	}
}

void ControlCueDialog::populateTargetCues()
{
	CueModelItem* parent = m_cue->parent();
	for ( int row = 0; row != parent->row( m_cue ); ++row )
	{
		CueModelItem* item = parent->child( row );
		if ( item->getType() == CueType_Audio )
		{
			m_target_cue->addItem( getName( item ), item->getUuid() );
		}
	}
}

} /* namespace noises */

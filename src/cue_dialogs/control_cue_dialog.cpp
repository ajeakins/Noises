
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QTimeEdit>

#include <cue/cue_model_item.h>
#include <cue_widget/types.h>

#include <utils/time.h>

#include "control_cue_dialog.h"

namespace noises
{

// VolumeChangeWidget

VolumeChangeWidget::VolumeChangeWidget( QWidget* parent )
:
	QWidget( parent )
{
	createWidgets();
}

void VolumeChangeWidget::createWidgets()
{
	QLabel* wait_time_label = new QLabel( "Fade time:", this );

	m_wait_time = new QTimeEdit( this );
	m_wait_time->setDisplayFormat( utils::defaultTimeFormat() );

	QLabel* stop_cue_on_end_label = new QLabel( "Stop cue on end:" );

	m_stop_cue_on_end =  new QCheckBox( this );

	QGridLayout* layout = new QGridLayout;
	layout->setContentsMargins( 0, 0, 0, 0 );
	layout->addWidget( wait_time_label, 0, 0 );
	layout->addWidget( m_wait_time, 0, 1 );
	layout->addWidget( stop_cue_on_end_label, 1, 0 );
	layout->addWidget( m_stop_cue_on_end, 1, 1 );

	setLayout( layout );
}

void VolumeChangeWidget::readSettings( const ControlCueSettings& /*settings*/ )
{

}

void VolumeChangeWidget::writeSettings( ControlCueSettings& /*settings*/ )
{

}

// ControlCueDialog

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

	connect(
		m_action, static_cast< void ( QComboBox::* )( int ) >( &QComboBox::currentIndexChanged ),
		this, &ControlCueDialog::typeChanged );
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

	settings.cue_action = getActionType();
	settings.target_cue_uuid = m_target_cue->itemData( m_target_cue->currentIndex() ).toString();

	if ( m_type_specific_widget )
	{
		m_type_specific_widget->writeSettings( settings );
	}
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
		typeChanged();
	}
}

ControlAction ControlCueDialog::getActionType() const
{
	return ( ControlAction )m_action->itemData( m_action->currentIndex() ).toInt();
}

void ControlCueDialog::createCueWidgets()
{
	createWidgets();

	QLabel* target_cue_label = new QLabel( "Target Cue:", this );
	m_target_cue = new QComboBox( this );
	populateTargetCues();

	QLabel* action_label = new QLabel( "Action", this );
	m_action = new QComboBox( this );

	ControlAction action = ( ControlAction )0;
	for ( ; action != ControlAction_ITEM_COUNT; ++action )
	{
		m_action->addItem( actionToString( action ), action );
	}

	QGridLayout* layout = new QGridLayout;
	layout->setContentsMargins( 0, 0, 0, 0 );

	layout->addWidget( target_cue_label, 0, 0 );
	layout->addWidget( m_target_cue, 0, 1 );

	layout->addWidget( action_label, 1, 0 );
	layout->addWidget( m_action, 1, 1 );

	m_type_specific_layout = new QVBoxLayout;

	QVBoxLayout* main_layout = new QVBoxLayout;
	main_layout->addLayout( layout );
	main_layout->addLayout( m_type_specific_layout );
	main_layout->addStretch();

	QGroupBox* group_box = new QGroupBox( "Control Settings", this );
	group_box->setLayout( main_layout );

	m_layout->addWidget( group_box );
}

void ControlCueDialog::typeChanged()
{
	// remove old item
	if ( m_type_specific_widget )
	{
		m_type_specific_layout->removeWidget( m_type_specific_widget );
		m_type_specific_widget->deleteLater();
		m_type_specific_widget = nullptr;
	}

	// create new widget if needed
	ControlAction action = getActionType();
	if ( action == ControlAction_VolumeChange )
	{
		m_type_specific_widget = new VolumeChangeWidget( this );
		m_type_specific_layout->addWidget( m_type_specific_widget );
	}
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

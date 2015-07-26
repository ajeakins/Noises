
#include <assert.h>

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QTimeEdit>

#include <cue/cue_model_item.h>
#include <cue/audio_cue_model_item.h>

#include <cue_widget/types.h>

#include <utils/time.h>

#include <widgets/matrix.h>

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

	m_fade_time = new QTimeEdit( this );
	m_fade_time->setDisplayFormat( utils::defaultTimeFormat() );

	QHBoxLayout* fade_row = new QHBoxLayout;
	fade_row->setContentsMargins( 0, 0, 0, 0 );
	fade_row->addWidget( wait_time_label );
	fade_row->addWidget( m_fade_time );

	m_stop_cue_on_end =  new QCheckBox( "Stop On End", this );
	m_stop_cue_on_end->setToolTip( "Stop target cue once fade is done" );

	// TODO need to read this from the target cue...
	m_matrix = new widgets::Matrix( 2, 2, this );

	QVBoxLayout* main_layout = new QVBoxLayout;
	main_layout->setContentsMargins( 0, 0, 0, 0 );
	main_layout->addLayout( fade_row );
	main_layout->addWidget( m_stop_cue_on_end );
	main_layout->addWidget( m_matrix );

	setLayout( main_layout );
}

void VolumeChangeWidget::readSettings( const ControlCueSettings::Ptr& settings )
{
	const VolumeChangeControlCueSettings::Ptr& volume_change_settings = \
		( const VolumeChangeControlCueSettings::Ptr& )( settings );

	m_fade_time->setTime( volume_change_settings->fade_time );
	m_stop_cue_on_end->setChecked( volume_change_settings->stop_target_on_end );

	m_matrix->readSettings( volume_change_settings->target_levels );
}

bool VolumeChangeWidget::writeSettings( ControlCueSettings::Ptr& settings )
{
	VolumeChangeControlCueSettings::Ptr& volume_change_settings = \
		( VolumeChangeControlCueSettings::Ptr& )( settings );

	bool something_changed = false;

	NOISES_SET_VALUE(
		volume_change_settings->fade_time,
		m_fade_time->time(),
		something_changed )
	NOISES_SET_VALUE(
		volume_change_settings->stop_target_on_end,
		m_stop_cue_on_end->isChecked(),
		something_changed )

	something_changed |= m_matrix->writeSettings( volume_change_settings->target_levels );

	return something_changed;
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

bool ControlCueDialog::writeSettings() const
{
	bool something_changed = false;

	ControlCueSettings::Ptr& settings = m_cue->getSettings();

	// check if we need to specialise the settings
	// TODO make this more generic
	if ( settings->cue_action != getActionType() && getActionType() == ControlAction_VolumeChange )
	{
		settings.reset( new VolumeChangeControlCueSettings );
		something_changed = true;
	}

	NOISES_SET_VALUE( settings->cue_action, getActionType(), something_changed )
	NOISES_SET_VALUE( settings->target_cue_uuid, getTargetCue(), something_changed )

	if ( m_type_specific_widget )
	{
		something_changed |= m_type_specific_widget->writeSettings( settings );
	}

	return something_changed;
}

void ControlCueDialog::readSettings()
{
	const ControlCueSettings::Ptr& settings = m_cue->getSettings();

	int index = m_action->findData( settings->cue_action );
	if ( index >= 0 )
	{
		m_action->setCurrentIndex( index );
		typeChanged();
	}

	index = m_target_cue->findData( settings->target_cue_uuid );
	if ( index >= 0 )
	{
		m_target_cue->setCurrentIndex( index );
	}

	if ( m_type_specific_widget )
	{
		m_type_specific_widget->readSettings( settings );
	}
}

ControlAction ControlCueDialog::getActionType() const
{
	return ( ControlAction )m_action->itemData( m_action->currentIndex() ).toInt();
}

QString ControlCueDialog::getTargetCue() const
{
	return m_target_cue->itemData( m_target_cue->currentIndex() ).toString();
}

void ControlCueDialog::createCueWidgets()
{
	createWidgets();

	QLabel* target_cue_label = new QLabel( "Target Cue:", this );
	m_target_cue = new QComboBox( this );
	populateTargetCues();
	connect(
		m_target_cue, static_cast< void ( QComboBox::* )( int ) >( &QComboBox::currentIndexChanged ),
		this, &ControlCueDialog::targetCueChanged );

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

void ControlCueDialog::targetCueChanged( int index )
{
	ControlAction action = getActionType();
	if ( action == ControlAction_VolumeChange )
	{
		QString uuid = m_target_cue->itemData( index ).toString();

		// read volume data from target cue as a starting point
		CueModelItem* parent = m_cue->parent();
		for ( int row = 0; row != parent->row( m_cue ); ++row )
		{
			CueModelItem* item = parent->child( row );
			if ( item->getUuid() == uuid )
			{
				assert( item->getType() == CueType_Audio );
				auto audio_cue = dynamic_cast< AudioCueModelItem* >( item );
				m_type_specific_widget->m_matrix->readSettings( audio_cue->getSettings().levels );
			}
		}
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
	CueModelItem* parent = m_cue->parent(); // No, no, no
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

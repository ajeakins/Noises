
#include <assert.h>

#include <QAbstractItemModel>
#include <QComboBox>
#include <QDataWidgetMapper>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>

#include <app/main_window.h>

#include <cue_widget/cue_model.h>
#include <cue_widget/types.h>

#include <cue/cue_model_item.h>

#include "cue_dialog.h"

namespace noises
{

CueDialog::CueDialog(
	QDataWidgetMapper* mapper,
	bool create,
	QWidget* parent )
:
	QDialog( parent ),
	m_mapper( mapper ),
	m_create( create )
{}

CueDialog::~CueDialog()
{}

void CueDialog::accept()
{
	m_mapper->submit();
	m_settings_changed |= writeSettings();

	QDialog::accept();
}

void CueDialog::createWidgets()
{
	// take ownership of the mapper
	m_mapper->setParent( this );

	// generic data from the model

	QGridLayout* generic_data_layout = new QGridLayout;
	generic_data_layout->setContentsMargins( 0, 0, 0, 0 );

	QAbstractItemModel* model = m_mapper->model();
	CueModel* cue_model = dynamic_cast< CueModel* >( model );

	int i = Column_Cue;
	for ( ; i <= Column_Notes; ++i )
	{
		QVariant header_data = model->headerData( i, Qt::Horizontal );
		QLabel* label = new QLabel( header_data.toString() );

		QLineEdit* editor = new QLineEdit;
		m_mapper->addMapping( editor, i );

		// for some reason we have to fill this in manually
		// but it get written correctly...
		QModelIndex index = model->index( m_mapper->currentIndex(), i );
		QVariant data = model->data( index );

		editor->setText( data.toString() );

		generic_data_layout->addWidget( label, i, 0 );
		generic_data_layout->addWidget( editor, i, 1 );
	}

	//
	// TODO make more generic :-( ----------------------------------------------
	//

	QVariant header_data = model->headerData( Column_PostAction, Qt::Horizontal );
	QLabel* post_action_label = new QLabel( header_data.toString() );

	QComboBox* post_action_editor = new QComboBox( this );
	m_mapper->addMapping( post_action_editor, Column_PostAction );

	QModelIndex index = model->index( m_mapper->currentIndex(), i );
	CueModelItem* item = cue_model->itemFromIndex( index );

	PostActions actions = item->getSupportedPostActions();
	for( PostAction j = (PostAction)0; j != PostAction_ITEM_COUNT; ++j)
	{
		if ( actions & j )
		{
			post_action_editor->addItem( postActionToString( j ), j );
		}
	}

	index = model->index( m_mapper->currentIndex(), Column_PostAction );
	int j = post_action_editor->findText( model->data( index ).toString() );
	if ( j >= 0 )
	{
		post_action_editor->setCurrentIndex( j );
	}

	generic_data_layout->addWidget( post_action_label, i, 0 );
	generic_data_layout->addWidget( post_action_editor, i, 1 );

	//
	// TODO --------------------------------------------------------------------
	//

	QGroupBox* generic_data_group_box = new QGroupBox( "Cue" );
	generic_data_group_box->setLayout( generic_data_layout );

	// buttons
	QDialogButtonBox::StandardButtons available_buttons = QDialogButtonBox::Ok;
	if ( !m_create )
	{
		available_buttons |= QDialogButtonBox::Cancel;
	}
	QDialogButtonBox* buttons = new QDialogButtonBox( available_buttons );

	connect(
		buttons, SIGNAL( accepted() ),
		this, SLOT( accept() ) );

	connect(
		buttons, SIGNAL( rejected() ),
		this, SLOT( reject() ) );

	m_layout = new QVBoxLayout;
	m_layout->setContentsMargins( 0, 0, 0, 0 );
	m_layout->addWidget( generic_data_group_box );

	QVBoxLayout* wrapping_layout = new QVBoxLayout;
	wrapping_layout->addItem( m_layout );
	wrapping_layout->addWidget( buttons );

	setLayout( wrapping_layout );
}

QLineEdit* CueDialog::getEditor( int section )
{
	QWidget* widget = m_mapper->mappedWidgetAt( section );
	assert( widget );

	return dynamic_cast< QLineEdit* >( widget );
}

} /* namespace noises */
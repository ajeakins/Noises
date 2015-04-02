
#include <assert.h>

#include <QAbstractItemModel>
#include <QComboBox>
#include <QDataWidgetMapper>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>

#include <cue_widget/cue_model.h>

#include "cue_dialog.h"

namespace noises
{

CueDialog::CueDialog(
	QDataWidgetMapper* mapper,
	QWidget* parent )
:
	QDialog( parent ),
	m_mapper( mapper )
{}

CueDialog::~CueDialog()
{}

void CueDialog::accept()
{
	m_mapper->submit();
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

	post_action_editor->addItem(
		postActionToString( PostAction_Advance ),
		PostAction_Advance );
	post_action_editor->addItem(
		postActionToString( PostAction_AdvanceAndPlay ),
		PostAction_AdvanceAndPlay );

	QModelIndex index = model->index( m_mapper->currentIndex(), Column_PostAction );
	post_action_editor->setCurrentIndex( model->data( index ).toInt() );

	generic_data_layout->addWidget( post_action_label, i, 0 );
	generic_data_layout->addWidget( post_action_editor, i, 1 );

	//
	// TODO --------------------------------------------------------------------
	//

	QGroupBox* generic_data_group_box = new QGroupBox( "Cue" );
	generic_data_group_box->setLayout( generic_data_layout );

	// buttons
	auto available_buttons = QDialogButtonBox::Ok | QDialogButtonBox::Cancel;
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
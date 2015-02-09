
#include <assert.h>

#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>

#include "cue_dialog.h"

namespace noises
{

CueDialog::CueDialog( QDataWidgetMapper* mapper, QWidget* parent )
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

	// genric data from the model

	QGridLayout* generic_data_layout = new QGridLayout;
	generic_data_layout->setContentsMargins( 0, 0, 0, 0 );

	int column_count = m_mapper->model()->columnCount();
	for ( int i = 0; i != column_count; ++i )
	{
		QAbstractItemModel* model = m_mapper->model();

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

	QGroupBox* generic_data_group_box = new QGroupBox( "Cue" );
	generic_data_group_box->setLayout( generic_data_layout );

	// buttons

	QDialogButtonBox* buttons = new QDialogButtonBox(
		QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

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
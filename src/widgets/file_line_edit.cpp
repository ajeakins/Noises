
#include <QApplication>
#include <QFileDialog>
#include <QStyle>

#include "file_line_edit.h"

namespace noises
{
namespace widgets
{

FileLineEdit::FileLineEdit( QWidget* parent )
:
	QWidget( parent )
{
	createWidgets();
}

FileLineEdit::~FileLineEdit()
{}

void FileLineEdit::createWidgets()
{
	m_line_edit = new QLineEdit( this );

	connect(
		m_line_edit, SIGNAL( editingFinished() ),
		this, SLOT( onEditingFinished() )
		);

	m_edit_button = new QPushButton( this );
	m_edit_button->setIcon( QIcon( ":images/folder_32x32.png" ) );

	connect(
		m_edit_button, SIGNAL( released() ),
		this, SLOT( showFileDialog() )
		);

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins( 0, 0, 0, 0 );

	layout->addWidget( m_line_edit );
	layout->addWidget( m_edit_button );

	setLayout( layout );
}

void FileLineEdit::showFileDialog()
{
	QFileDialog dialog( this );
	dialog.exec();

	if ( dialog.result() == QDialog::Accepted && !dialog.selectedFiles().isEmpty() )
	{
		m_line_edit->setText( dialog.selectedFiles()[0] );
		Q_EMIT filenameChanged();
	}
}

void FileLineEdit::onEditingFinished()
{
	Q_EMIT filenameChanged();
}

} /* namespace widgets */
} /* namespace noises */

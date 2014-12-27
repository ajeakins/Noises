#pragma once

#include <QWidget>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>

namespace noises
{
namespace widgets
{
	class FileLineEdit : public QWidget
	{
		Q_OBJECT

	public:
		FileLineEdit( QWidget* parent = 0 );

		~FileLineEdit();

		QString text()
		{
			return m_line_edit->text();
		}

		void setText( const QString& text )
		{
			m_line_edit->setText( text );
		}

	private Q_SLOTS:
		void showFileDialog();

	private:
		void createWidgets();

	private:
		QLineEdit* m_line_edit;
		QPushButton* m_edit_button;
	};

} /* namespace widgets */
} /* namespace noises */

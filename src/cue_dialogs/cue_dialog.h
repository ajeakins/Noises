#pragma once

#include <iostream>

#include <QDialog>
#include <QLineEdit>

class QDataWidgetMapper;
class QLayout;

namespace noises
{
	class CueDialog: public QDialog
	{
		Q_OBJECT

	public:
		CueDialog( QDataWidgetMapper* mapper, QWidget* parent = 0 );

		virtual ~CueDialog();

	public Q_SLOTS:
		virtual void accept();

	protected:
		void createWidgets();

		QLineEdit* getEditor( int section );

	protected:
		QLayout* m_layout;

	private:
		QDataWidgetMapper* m_mapper;
	};

} /* namespace noises */
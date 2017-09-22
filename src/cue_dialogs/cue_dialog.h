#pragma once

#include <QDialog>
#include <QLineEdit>

#include <cue/types.h>

class QDataWidgetMapper;
class QLayout;

namespace noises
{
	class CueDialog: public QDialog
	{
		Q_OBJECT

	public:
		CueDialog(
			QDataWidgetMapper* mapper,
			bool create, // TODO: this is horrific
			QWidget* parent = nullptr );

		virtual ~CueDialog();

		bool settingsChanged()
		{
			return m_settings_changed;
		}

	public Q_SLOTS:
		void accept();

	protected:
		void createWidgets();

		void setDialogTitle( CueType type );

		QLineEdit* getEditor( int section );

		virtual void readSettings() = 0;

		virtual bool writeSettings() const = 0;

	protected:
		QLayout* m_layout = nullptr;

	private:
		QDataWidgetMapper* m_mapper;

		bool m_create;
		bool m_settings_changed = false;
	};

} /* namespace noises */

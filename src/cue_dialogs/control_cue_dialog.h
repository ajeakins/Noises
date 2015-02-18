#pragma once

#include "cue_dialog.h"

class QDataWidgetMapper;

namespace noises
{
	class ControlCueModelItem;

	class ControlCueDialog: public CueDialog
	{
		Q_OBJECT

	public:
		ControlCueDialog(
			ControlCueModelItem* cue,
			QDataWidgetMapper* mapper,
			QWidget* parent = nullptr );

		virtual ~ControlCueDialog();

	public Q_SLOTS:
		virtual void accept();

	private:
		void createCueWidgets();

		void readSettings();

		void writeSettings();

	private:

		// cue data

		ControlCueModelItem* m_cue;

		// widgets

	};

} /* namespace noises */

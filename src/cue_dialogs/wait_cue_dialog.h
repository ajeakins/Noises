#pragma once

#include <cues/wait_cue_model_item.h>

#include "cue_dialog.h"

class QDataWidgetMapper;

namespace noises
{
	class WaitCueDialog: public CueDialog
	{
		Q_OBJECT

	public:
		WaitCueDialog(
			WaitCueModelItem* cue,
			QDataWidgetMapper* mapper,
			QWidget* parent = 0 );

		virtual ~WaitCueDialog();

	public Q_SLOTS:
		virtual void accept();

	private:
		void createCueWidgets();

		void readSettings();

		void writeSettings();

	private:

		// cue data

		WaitCueModelItem* m_cue;

		// widgets

	};

} /* namespace noises */
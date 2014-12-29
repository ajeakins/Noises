
#pragma once

#include <cues/control_cue_model_item.h>

#include "cue_dialog.h"

class QDoubleSpinBox;
class QDataWidgetMapper;

namespace noises
{
	class ControlCueDialog: public CueDialog
	{
		Q_OBJECT

	public:
		ControlCueDialog(
			ControlCueModelItem* cue,
			QDataWidgetMapper* mapper,
			QWidget* parent = 0 );

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
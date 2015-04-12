#pragma once

#include "cue_dialog.h"

class QDataWidgetMapper;
class QComboBox;

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
			bool create,
			QWidget* parent = nullptr );

		virtual ~ControlCueDialog();

	public Q_SLOTS:
		virtual void accept();

	private:
		void createCueWidgets();

		void populateTargetCues();

		void readSettings();

		void writeSettings();

	private:

		// cue data

		ControlCueModelItem* m_cue;

		// widgets

		QComboBox* m_target_cue;

		QComboBox* m_action;

	};

} /* namespace noises */

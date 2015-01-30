#pragma once

#include <cues/group_cue_model_item.h>

#include "cue_dialog.h"

class QDataWidgetMapper;

namespace noises
{
	class GroupCueDialog: public CueDialog
	{
		Q_OBJECT

	public:
		GroupCueDialog(
			GroupCueModelItem* cue,
			QDataWidgetMapper* mapper,
			QWidget* parent = 0 );

		virtual ~GroupCueDialog();

	public Q_SLOTS:
		virtual void accept();

	private:
		void createCueWidgets();

		void readSettings();

		void writeSettings();

	private:

		// cue data

		GroupCueModelItem* m_cue;

		// widgets

	};

} /* namespace noises */

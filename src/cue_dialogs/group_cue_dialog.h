#pragma once

#include "cue_dialog.h"

class QDataWidgetMapper;

namespace noises
{
	class GroupCueModelItem;

	class GroupCueDialog: public CueDialog
	{
		Q_OBJECT

	public:
		GroupCueDialog(
			GroupCueModelItem* cue,
			QDataWidgetMapper* mapper,
			QWidget* parent = nullptr );

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

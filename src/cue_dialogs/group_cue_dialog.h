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
			bool create,
			QWidget* parent = nullptr );

		virtual ~GroupCueDialog();

	private:
		void createCueWidgets();

		void readSettings() override;

		bool writeSettings() const override;

	private:

		// cue data

		GroupCueModelItem* m_cue;

		// widgets

	};

} /* namespace noises */

#pragma once

#include "cue_dialog.h"

class QDataWidgetMapper;
class QTimeEdit;

namespace noises
{
	class WaitCueModelItem;

	class WaitCueDialog: public CueDialog
	{
		Q_OBJECT

	public:
		WaitCueDialog(
			WaitCueModelItem* cue,
			QDataWidgetMapper* mapper,
			bool create,
			QWidget* parent = nullptr );

		virtual ~WaitCueDialog();

	private:
		void createCueWidgets();

		void readSettings() override;

		bool writeSettings() const override;

	private:

		// cue data

		WaitCueModelItem* m_cue;

		// widgets

		QTimeEdit* m_wait_time;

	};

} /* namespace noises */

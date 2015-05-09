#pragma once

#include <cue/control_cue_model_item.h>

#include "cue_dialog.h"

class QDataWidgetMapper;
class QCheckBox;
class QComboBox;
class QTimeEdit;
class QVBoxLayout;

namespace noises
{
	namespace widgets
	{
		class Matrix;
	}

	class ControlCueModelItem;

	class VolumeChangeWidget: public QWidget
	{
		Q_OBJECT

	public:
		VolumeChangeWidget( QWidget* parent = nullptr );

		void readSettings( const ControlCueSettings::Ptr& settings );

		bool writeSettings( ControlCueSettings::Ptr& settings );

	private:
		void createWidgets();

	private:
		QTimeEdit* m_fade_time;
		QCheckBox* m_stop_cue_on_end;
		QPushButton* m_reset;
		widgets::Matrix* m_matrix;
	};

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

	private Q_SLOTS:
		void typeChanged();

	private:
		void createCueWidgets();

		void populateTargetCues();

		ControlAction getActionType() const;

		QString getTargetCue() const;

		void readSettings() override;

		bool writeSettings() const override;

	private:

		// cue data

		ControlCueModelItem* m_cue;

		// widgets

		QComboBox* m_target_cue;
		QComboBox* m_action;

		QVBoxLayout* m_type_specific_layout;
		VolumeChangeWidget* m_type_specific_widget = nullptr;
	};

} /* namespace noises */

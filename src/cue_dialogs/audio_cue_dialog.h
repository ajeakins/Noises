#pragma once

#include <audio/player.h>

#include <cues/audio_cue_model_item.h>

#include <widgets/file_line_edit.h>
#include <widgets/matrix.h>

#include "cue_dialog.h"

class QDoubleSpinBox;
class QDataWidgetMapper;

namespace noises
{
	class AudioCueDialog: public CueDialog
	{
		Q_OBJECT

	public:
		AudioCueDialog(
			AudioCueModelItem* cue,
			QDataWidgetMapper* mapper,
			QWidget* parent = 0 );

		virtual ~AudioCueDialog();

	public Q_SLOTS:
		virtual void accept();

		void play();

		void stop();

		void onFilenameChanged();

	private:
		void createCueWidgets();

		void readSettings();

		void writeSettings();

	private:

		// cue data

		AudioCueModelItem* m_cue;

		// audio player

		audio::Player::Ptr m_player;

		// widgets

		widgets::FileLineEdit* m_file_edit;

		QPushButton* m_play_button;
		QPushButton* m_stop_button;

		QPushButton* m_reset_times_button;

		QDoubleSpinBox* m_start_time;
		QDoubleSpinBox* m_end_time;
		QDoubleSpinBox* m_fade_in_time;
		QDoubleSpinBox* m_fade_out_time;

		widgets::Matrix* m_matrix;

	};

} /* namespace noises */
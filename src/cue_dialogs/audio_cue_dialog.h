#pragma once

#include <audio/player.h>

#include <cues/audio_cue_model_item.h>

#include <widgets/file_line_edit.h>
#include <widgets/matrix.h>

#include "cue_dialog.h"

class QTimeEdit;
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

		void resetTimes();

	private:
		void createCueWidgets();

		void setTimeDisplayFormat( QString format );

		void readSettings();

		void writeSettings() const;

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

		QTimeEdit* m_start_time;
		QTimeEdit* m_end_time;
		QTimeEdit* m_fade_in_time;
		QTimeEdit* m_fade_out_time;

		widgets::Matrix* m_matrix;

	};

} /* namespace noises */
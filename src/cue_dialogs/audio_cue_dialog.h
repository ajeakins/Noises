#pragma once

#include <audio/audio_player.h>

#include "cue_dialog.h"

class QTimeEdit;
class QDataWidgetMapper;

namespace noises
{
	class AudioCueModelItem;

	namespace widgets
	{
		class Matrix;
		class FileLineEdit;
	}

	class AudioCueDialog: public CueDialog
	{
		Q_OBJECT

	public:
		AudioCueDialog(
			AudioCueModelItem* cue,
			QDataWidgetMapper* mapper,
			bool create,
			QWidget* parent = nullptr );

		virtual ~AudioCueDialog();

	private Q_SLOTS:
		virtual void accept();

		void onFilenameChanged();

		void resetTimes();

		void volumeChanged();

		void playerTimeChanged( const QTime& time );

	private:
		void createCueWidgets();

		void setTimeDisplayFormat( QString format );

		void readSettings();

		void writeSettings() const;

	private:

		// cue data

		AudioCueModelItem* m_cue;

		// audio player

		audio::AudioPlayer::Ptr m_player;
		QTime m_duration;

		// widgets

		widgets::FileLineEdit* m_file_edit;

		QPushButton* m_play_button;
		QPushButton* m_pause_button;
		QPushButton* m_stop_button;

		QTimeEdit* m_remaining_time;
		QTimeEdit* m_elapsed_time;

		QPushButton* m_reset_times_button;

		QTimeEdit* m_start_time;
		QTimeEdit* m_end_time;
		QTimeEdit* m_fade_in_time;
		QTimeEdit* m_fade_out_time;

		widgets::Matrix* m_matrix;

	};

} /* namespace noises */

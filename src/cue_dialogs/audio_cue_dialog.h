#pragma once

#include <audio/player/audio_player.h>

#include "cue_dialog.h"

class QSlider;
class QTimeEdit;
class QDataWidgetMapper;

namespace noises
{
	class AudioCueModelItem;

	namespace widgets
	{
		class Matrix;
		class FileLineEdit;
		class Pan;
		class TimeLabel;
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
		void onFilenameChanged();

		void resetTimes();

		void volumeChanged();

		void playerTimeChanged( const QTime& time );

		void playPause();

		void stop();

	private:
		void createCueWidgets();

		void setTimeDisplayFormat( QString format );

		void readSettings() override;

		bool writeSettings() const override;

	private:

		// cue data

		AudioCueModelItem* m_cue;

		// audio player

		audio::AudioPlayer::Ptr m_player;
		QTime m_duration;

		// widgets

		widgets::FileLineEdit* m_file_edit;

		QPushButton* m_play_pause_button;
		QPushButton* m_stop_button;

		widgets::TimeLabel* m_remaining_time;
		widgets::TimeLabel* m_elapsed_time;

		QPushButton* m_reset_times_button;

		QTimeEdit* m_start_time;
		QTimeEdit* m_end_time;
		QTimeEdit* m_fade_in_time;
		QTimeEdit* m_fade_out_time;

		widgets::Matrix* m_matrix;

	};

} /* namespace noises */

#pragma once

#include <QTime>

#include <audio/player/audio_player.h>
#include <audio/volume_matrix.h>
#include <app/application.h>

#include "cue_model_item.h"
#include "types.h"

namespace noises
{
	struct AudioCueSettings
	{
	public:
		AudioCueSettings()
		{
			bool link = Application::getPreferences().getDefaultStereoLink();
			levels.setStereoLink( link );
		}

		void readSettings( const QJsonObject& root );

		void writeSettings( QJsonObject& root ) const;

	public:
		QString file_name;

		QTime start_time, end_time;
		QTime start_fade, end_fade;

		audio::VolumeMatrix levels;
	};

	class AudioCueModelItem: public CueModelItem
	{
	public:
		AudioCueModelItem(
			const QList< QVariant >& item_data,
			CueModelItem* parent_item = nullptr );

		~AudioCueModelItem();

		CueType getType() const override
		{
			return CueType_Audio;
		}

		AudioCueSettings& getSettings()
		{
			return m_settings;
		}

		void execute() override;

		void readSettings( const QJsonObject& settings ) override;

		void writeSettings( QJsonObject& settings ) const override;

		bool hasDuration() const override
		{
			return true;
		}

		QString getTimeFormat() const override
		{
			return m_time_format;
		}

		QTime getDuration() const override
		{
			return m_duration;
		}

		audio::AudioPlayer::Ptr getPlayer()
		{
			return m_player;
		}

		// Shouldn't need this
		void updatePlayer();

	protected:
		QVariant getIcon() const override;

	private Q_SLOTS:
		void playerTimeChanged( const QTime& time );

	private:
		AudioCueSettings m_settings;
		QTime m_duration;
		QString m_time_format;

		audio::AudioPlayer::Ptr m_player;
	};

} /* namespace noises */

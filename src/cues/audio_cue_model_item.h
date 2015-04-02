#pragma once

#include <QTime>

#include <audio/player.h>
#include <app/application.h>
#include <widgets/matrix.h>

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

		widgets::MatrixSettings levels;
	};

	class AudioCueModelItem: public CueModelItem
	{
	public:
		AudioCueModelItem(
			const QList< QVariant >& item_data,
			CueModelItem* parent_item = nullptr );

		virtual ~AudioCueModelItem();

		virtual CueType getType() const
		{
			return CueType_Audio;
		}

		AudioCueSettings& getSettings()
		{
			return m_settings;
		}

		void execute() const;

		void readSettings( const QJsonObject& settings );

		void writeSettings( QJsonObject& settings ) const;

		// TODO: move these up class hiearchy, should be shared amoungst all cues
		// that have the concept of duration

		float getProgress() const;

		const QString& getTimeFormat() const { return m_time_format; }

		const QTime& getDuration() const { return m_duration; }

		// Shouldn't need this
		void updatePlayer();

	protected:
		QVariant getIcon() const;

	private Q_SLOTS:
		void playerTimeChanged( const QTime& time );

	private:
		AudioCueSettings m_settings;
		QTime m_duration;
		QString m_time_format;

		audio::Player::Ptr m_player;
	};

} /* namespace noises */

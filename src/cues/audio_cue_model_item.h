#pragma once

#include <iostream>

#include <QTime>

#include <audio/player.h>
#include <application.h>
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
			CueModelItem* parent_item = 0 );

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

	protected:
		QVariant getIcon() const;

	private:
		AudioCueSettings m_settings;

		audio::Player::Ptr m_player;
	};

} /* namespace noises */

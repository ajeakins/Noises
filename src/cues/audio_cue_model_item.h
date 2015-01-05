#pragma once

#include <iostream>

#include <application.h>
#include <widgets/matrix.h>

#include "cue_model_item.h"
#include "types.h"

namespace noises
{
	struct AudioCueSettings
	{
		AudioCueSettings()
		:
			start_time( 0.0f ),
			end_time( 0.0f ),
			start_fade( 0.0f ),
			end_fade( 0.0f )
		{
			bool link = Application::getPreferences().defaultStereoLink();
			levels.setStereoLink( link );
		}

		QString file_name;

		float start_time, end_time;
		float start_fade, end_fade;

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
			return cue_settings;
		}

		void execute() const;

	protected:
		virtual QVariant getIcon() const;

	private:
		AudioCueSettings cue_settings;
	};

} /* namespace noises */

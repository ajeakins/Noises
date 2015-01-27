#pragma once

#include <audio/manager.h>

#include "preferences.h"

namespace noises
{
	// Wrapper for application globals
	class Application
	{
	public:
		Application();

		static Preferences& getPreferences();

		static audio::Manager& getAudioManager();

	private:
		static Preferences m_preferences;
		static audio::Manager m_audio_manager;
	};

} /* namespace noises */

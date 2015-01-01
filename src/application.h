#pragma once

#include "preferences.h"

namespace noises
{
	// Wrapper for application globals
	class Application
	{
	public:
		Application();

		static Preferences& getPreferences();

	private:
		static Preferences m_preferences;
	};

} /* namespace noises */

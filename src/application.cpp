
#include "application.h"

namespace noises
{

Preferences Application::m_preferences;

Application::Application()
{}

Preferences& Application::getPreferences()
{
	return m_preferences;
}

} /* namespace noises */
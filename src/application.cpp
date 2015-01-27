
#include "application.h"

namespace noises
{

Preferences Application::m_preferences;
audio::Manager Application::m_audio_manager;

Application::Application()
{}

Preferences& Application::getPreferences()
{
	return m_preferences;
}

audio::Manager& Application::getAudioManager()
{
	return m_audio_manager;
}

} /* namespace noises */

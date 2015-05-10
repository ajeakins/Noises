
#include <app/application.h>
#include <utils/time.h>

#include "fade_player.h"

namespace noises
{
namespace audio
{

FadePlayer::FadePlayer( QObject* parent )
:
	Player( parent )
{}

FadePlayer::~FadePlayer()
{}

void FadePlayer::start()
{
	if ( m_is_playing )
	{
		return;
	}

	// compute length
	m_length = utils::timeToMsecs( m_duration );
	m_length *= Application::getPreferences().getSampleRate();
	m_length /= 1000;

	m_pos = 0;

	m_is_playing = true;
	Q_EMIT started( sharedFromThis() );
}

void FadePlayer::stop()
{
	m_is_playing = false;
	m_pos = 0;
	Q_EMIT stopped();
}

void FadePlayer::updateTime() const
{
	float timeInMSecs = ( float )m_pos;
	timeInMSecs *= 1000.0f;
	timeInMSecs /= Application::getPreferences().getSampleRate();

	QTime time( 0, 0 ,0 );
	time = time.addMSecs( ( int )timeInMSecs );

	Q_EMIT Player::timeUpdated( time );
}

void FadePlayer::addData(
	float* /*audio_data*/,
	int frames,
	int /*channels*/ )
{
	if ( !m_is_playing )
	{
		return;
	}

	m_pos += frames;
	if ( m_pos > m_length )
	{
		m_is_playing = false;
		m_pos = 0;
		Q_EMIT fadeDone();
	}
}

} /* namespace audio */
} /* namespace noises */
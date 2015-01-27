
#include "audio_cue_model_item.h"

namespace noises
{

AudioCueModelItem::AudioCueModelItem(
	const QList< QVariant >& item_data,
	CueModelItem* parent_item )
:
		CueModelItem( item_data, parent_item ),
		m_player( audio::Player::create() )
{
	Application::getAudioManager().registerPlayer( m_player );
}

AudioCueModelItem::~AudioCueModelItem()
{
	Application::getAudioManager().unregisterPlayer( m_player );
}

void AudioCueModelItem::execute() const
{
	if( !m_settings.file_name.isEmpty() )
	{
		m_player->setFilename( m_settings.file_name );
		m_player->start();
	}
}

QVariant AudioCueModelItem::getIcon() const
{
	QPixmap icon( ":/images/audio_cue_16x16.png" );
	return icon;
}

} /* namespace noises */

#include "audio_cue_model_item.h"

namespace noises
{

void AudioCueSettings::readSettings( const Json::Value& value )
{
	assert( value.type() == Json::objectValue );

	file_name = value["file_name"].asCString();
}

void AudioCueSettings::writeSettings( Json::Value& value ) const
{
	assert( value.type() == Json::objectValue );

	value["file_name"] = file_name.toStdString();
}

AudioCueModelItem::AudioCueModelItem(
	const QList< QVariant >& item_data,
	CueModelItem* parent_item )
:
		CueModelItem( item_data, parent_item )
{
	m_player = Application::getAudioManager().createPlayer( this );
}

AudioCueModelItem::~AudioCueModelItem()
{}

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

void AudioCueModelItem::readSettings( const Json::Value& root )
{
	CueModelItem::readSettings( root );
	m_settings.readSettings( root["audio-settings"] );
}

void AudioCueModelItem::writeSettings( Json::Value& root ) const
{
	CueModelItem::writeSettings( root );

	Json::Value audioSettings( Json::objectValue );
	m_settings.writeSettings( audioSettings );
	root["audio-settings"] = audioSettings;

}

} /* namespace noises */
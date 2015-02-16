
#include "audio_cue_model_item.h"

namespace noises
{

void AudioCueSettings::readSettings( const QJsonObject& value )
{
	file_name = value["file_name"].toString( "" );
}

void AudioCueSettings::writeSettings( QJsonObject& value ) const
{
	value["file_name"] = file_name;
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

void AudioCueModelItem::readSettings( const QJsonObject& settings )
{
	CueModelItem::readSettings( settings );
	m_settings.readSettings( settings["audio-settings"].toObject() );
}

void AudioCueModelItem::writeSettings( QJsonObject& settings ) const
{
	CueModelItem::writeSettings( settings );

	QJsonObject audioSettings;
	m_settings.writeSettings( audioSettings );
	settings["audio-settings"] = audioSettings;

}

} /* namespace noises */
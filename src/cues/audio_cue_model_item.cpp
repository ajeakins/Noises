
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
	CueModelItem( item_data, parent_item ),
	m_time_format( utils::defaultTimeFormat() )
{
	m_player = Application::getAudioManager().createPlayer( this );

	connect(
		m_player.data(), &audio::Player::timeUpdated,
		this, &AudioCueModelItem::playerTimeChanged );
}

AudioCueModelItem::~AudioCueModelItem()
{}

void AudioCueModelItem::execute() const
{
	m_player->start();
}

QVariant AudioCueModelItem::getIcon() const
{
	QPixmap icon( ":/images/audio_cue_16x16.png" );
	return icon;
}

void AudioCueModelItem::updatePlayer()
{
	m_player->setFilename( m_settings.file_name );
	m_player->setVolume( m_settings.levels );

	m_duration = m_player->getDuration();

	m_time_format = utils::timeFormat( m_duration );
	playerTimeChanged( QTime() );
}

void AudioCueModelItem::playerTimeChanged( const QTime& time )
{
	QTime remaining = utils::subtract( m_duration, time );

	setData( 3, time.toString( m_time_format ) );
	setData( 4, remaining.toString( m_time_format ) );

	Q_EMIT dataChanged( this );
}

void AudioCueModelItem::readSettings( const QJsonObject& settings )
{
	CueModelItem::readSettings( settings );
	m_settings.readSettings( settings["audio-settings"].toObject() );

	updatePlayer();
}

void AudioCueModelItem::writeSettings( QJsonObject& settings ) const
{
	CueModelItem::writeSettings( settings );

	QJsonObject audioSettings;
	m_settings.writeSettings( audioSettings );
	settings["audio-settings"] = audioSettings;

}

} /* namespace noises */
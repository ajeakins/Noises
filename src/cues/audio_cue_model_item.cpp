
#include <QPixmap>

#include <utils/time.h>

#include "audio_cue_model_item.h"

namespace noises
{

void AudioCueSettings::readSettings( const QJsonObject& value )
{
	file_name = value["file_name"].toString( "" );

	start_time = utils::timeFromMsecs( value["start_time"].toInt() );
	end_time = utils::timeFromMsecs( value["end_time"].toInt() );

	start_fade = utils::timeFromMsecs( value["start_fade"].toInt() );
	end_fade = utils::timeFromMsecs( value["end_fade"].toInt() );

	QJsonObject levelSettings = value["levels"].toObject();
	levels.readSettings( levelSettings );
}

void AudioCueSettings::writeSettings( QJsonObject& value ) const
{
	value["file_name"] = file_name;

	value["start_time"] = utils::timeToMsecs( start_time );
	value["end_time"] = utils::timeToMsecs( end_time );

	value["start_fade"] = utils::timeToMsecs( start_fade );
	value["end_fade"] = utils::timeToMsecs( end_fade );

	QJsonObject levelSettings;
	levels.writeSettings( levelSettings );
	value["levels"] = levelSettings;
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
	playerTimeChanged( QTime( 0, 0, 0 ) );
}

void AudioCueModelItem::playerTimeChanged( const QTime& time )
{
	QTime remaining;
	if ( utils::isZero( m_duration ) )
	{
		remaining = QTime( 0, 0, 0 );
	}
	else
	{
		remaining = utils::subtract( m_duration, time );
	}

	setData( 3, remaining.toString( m_time_format ) );
	setData( 4, time.toString( m_time_format ) );

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

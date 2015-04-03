
#include <QPixmap>

#include <audio/manager.h>
#include <app/application.h>

#include <utils/time.h>

#include "wait_cue_model_item.h"

namespace noises
{

void WaitCueSettings::readSettings( const QJsonObject& value )
{
	wait_time = utils::timeFromMsecs( value["wait_time"].toInt() );
}

void WaitCueSettings::writeSettings( QJsonObject& value ) const
{
	value["wait_time"] = utils::timeToMsecs( wait_time );
}

WaitCueModelItem::WaitCueModelItem(
	const QList< QVariant >& item_data,
	CueModelItem* parent_item )
:
	CueModelItem( item_data, parent_item )
{
	audio::Manager& manager = Application::getAudioManager();
	audio::Player::Ptr player = manager.createPlayer( this, audio::PlayerType_Wait );
	m_player = player.dynamicCast< audio::WaitPlayer >();

	connect(
		m_player.data(), &audio::Player::timeUpdated,
		this, &WaitCueModelItem::playerTimeChanged );
}

WaitCueModelItem::~WaitCueModelItem()
{}

void WaitCueModelItem::execute() const
{
	m_player->start();
}

QVariant WaitCueModelItem::getIcon() const
{
	QPixmap icon( ":/images/wait_cue_16x16.png" );
	return icon;
}

void WaitCueModelItem::playerTimeChanged( const QTime& time )
{
	(void)time;

	// QTime remaining;
	// if ( utils::isZero( m_duration ) )
	// {
	// 	remaining = QTime( 0, 0, 0 );
	// }
	// else
	// {
	// 	remaining = utils::subtract( m_duration, time );
	// }

	// setData( 3, remaining.toString( m_time_format ) );
	// setData( 4, time.toString( m_time_format ) );

	// Q_EMIT dataChanged( this );
}


void WaitCueModelItem::readSettings( const QJsonObject& settings )
{
	CueModelItem::readSettings( settings );

	m_settings.readSettings( settings["wait-settings"].toObject() );

	updatePlayer();
}

void WaitCueModelItem::writeSettings( QJsonObject& settings ) const
{
	CueModelItem::writeSettings( settings );

	QJsonObject audioSettings;
	m_settings.writeSettings( audioSettings );
	settings["wait-settings"] = audioSettings;
}

void WaitCueModelItem::updatePlayer()
{
	m_player->setDuration( m_settings.wait_time );
}

} /* namespace noises */

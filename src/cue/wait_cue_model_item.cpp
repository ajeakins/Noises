
#include <QPixmap>

#include <audio/manager.h>
#include <app/application.h>
#include <cue_widget/types.h>

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
	audio::Player::Ptr player = manager.createPlayer( audio::PlayerType_Wait );
	m_player = player.dynamicCast< audio::WaitPlayer >();

	connect(
		m_player.data(), &audio::Player::timeUpdated,
		this, &WaitCueModelItem::playerTimeChanged );

	connect(
		m_player.data(), &audio::WaitPlayer::waitDone,
		this, &WaitCueModelItem::waitDone );
}

WaitCueModelItem::~WaitCueModelItem()
{}

void WaitCueModelItem::execute()
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
	QTime remaining;
	if ( utils::isZero( m_settings.wait_time ) )
	{
		remaining = QTime( 0, 0, 0 );
	}
	else
	{
		remaining = utils::subtract( m_settings.wait_time, time );
	}

	setData( Column_Remaining, remaining );
	setData( Column_Elapsed, time );

	Q_EMIT dataChanged( this );
}

void WaitCueModelItem::waitDone()
{
	Q_EMIT cueDone( this );

	const QString& post_action = data( Column_PostAction ).toString();
	if ( stringToPostAction( post_action ) == PostAction_AdvanceAndPlay )
	{
		int row = parent()->row( this ) + 1;
		if ( row < parent()->childCount() )
		{
			parent()->child( row )->execute();
		}
	}
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
	m_time_format = utils::timeFormat( m_settings.wait_time );

	playerTimeChanged( QTime( 0, 0, 0 ) );
}

} /* namespace noises */

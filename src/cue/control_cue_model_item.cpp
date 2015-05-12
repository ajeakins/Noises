
#include <QPixmap>
#include <QSharedPointer>

#include <cue_widget/types.h>

#include <utils/time.h>

#include "audio_cue_model_item.h"
#include "control_cue_model_item.h"

namespace noises
{

QString actionToString( ControlAction action )
{
	switch( action )
	{
		case ControlAction_Stop:
			return "Stop";
		case ControlAction_Start:
			return "Start";
		case ControlAction_Pause:
			return "Pause";
		case ControlAction_VolumeChange:
			return "Volume Change";
		case ControlAction_ITEM_COUNT:
			return "";
	}
	return "";
}

// ControlCueSettings

ControlCueSettings::~ControlCueSettings()
{}

void ControlCueSettings::readSettings( const QJsonObject& value )
{
	target_cue_uuid = value["target_cue_uuid"].toString();
	cue_action = ( ControlAction )value["cue_action"].toInt();
}

void ControlCueSettings::writeSettings( QJsonObject& value ) const
{
	value["target_cue_uuid"] = target_cue_uuid;
	value["cue_action"] = cue_action;
}

// VolumeChangeControlCueSettings

VolumeChangeControlCueSettings::~VolumeChangeControlCueSettings()
{}

void VolumeChangeControlCueSettings::readSettings( const QJsonObject& value )
{
	ControlCueSettings::readSettings( value );

	fade_time = utils::timeFromMsecs( value["fade_time"].toInt() );
	stop_target_on_end = value["stop_target_on_end"].toBool();

	QJsonObject target_levels_settings = value["target_levels"].toObject();
	target_levels.readSettings( target_levels_settings );
}

void VolumeChangeControlCueSettings::writeSettings( QJsonObject& value ) const
{
	ControlCueSettings::writeSettings( value );

	value["fade_time"] = utils::timeToMsecs( fade_time );
	value["stop_target_on_end"] = stop_target_on_end;

	QJsonObject target_levels_settings;
	target_levels.writeSettings( target_levels_settings );
	value["target_levels"] = target_levels_settings;
}

// ControlCueModelItem

ControlCueModelItem::ControlCueModelItem(
	const QList< QVariant >& item_data,
	CueModelItem* parent_item )
:
		CueModelItem( item_data, parent_item ),
		m_settings( new ControlCueSettings )
{}

ControlCueModelItem::~ControlCueModelItem()
{}

void ControlCueModelItem::execute()
{
	CueModelItem* parent = this->parent();
	CueModelItem* target = 0;
	for ( int i = 0; i != parent->row( this ); ++i )
	{
		CueModelItem* item = parent->child( i );
		if ( item->getUuid() == m_settings->target_cue_uuid )
		{
			target = item;
		}
	}

	if ( target && target->getType() == CueType_Audio )
	{
		AudioCueModelItem* audio_cue = ( AudioCueModelItem* )target;

		switch( m_settings->cue_action )
		{
			case ControlAction_Start:
				audio_cue->getPlayer()->start();
				break;
			case ControlAction_Stop:
				audio_cue->getPlayer()->stop();
				break;
			case ControlAction_Pause:
				audio_cue->getPlayer()->pause();
				break;
			case ControlAction_VolumeChange:
			{
				audio::Manager& manager = Application::getAudioManager();
				audio::Player::Ptr player = manager.createPlayer( this, audio::PlayerType_Fade );
				m_fade_player = player.dynamicCast< audio::FadePlayer >();

				connect(
					m_fade_player.data(), &audio::FadePlayer::timeUpdated,
					this, &ControlCueModelItem::playerTimeChanged );
				connect(
					m_fade_player.data(), &audio::FadePlayer::fadeDone,
					this, &ControlCueModelItem::fadeDone
					);

				auto volume_change_settings = dynamic_cast< VolumeChangeControlCueSettings* >( m_settings.data() );
				m_fade_player->setFadeTime( volume_change_settings->fade_time );
				m_fade_player->setTargetLevels( volume_change_settings->target_levels );

				audio_cue->getPlayer()->addFade( m_fade_player );
				break;
			}
			case ControlAction_ITEM_COUNT:
				break;
		}
	}

	Q_EMIT cueDone( this );
}

QVariant ControlCueModelItem::getIcon() const
{
	QPixmap icon( ":/images/control_cue_16x16.png" );
	return icon;
}

bool ControlCueModelItem::hasDuration() const
{
	if ( m_settings->cue_action == ControlAction_VolumeChange )
	{
		return true;
	}
	return false;
}

QString ControlCueModelItem::getTimeFormat() const
{
	if ( m_settings->cue_action == ControlAction_VolumeChange )
	{
		auto volume_change_settings = dynamic_cast< VolumeChangeControlCueSettings* >( m_settings.data() );
		return utils::timeFormat( volume_change_settings->fade_time );
	}
	return "";
}

QTime ControlCueModelItem::getDuration() const
{
	if ( m_settings->cue_action == ControlAction_VolumeChange )
	{
		auto volume_change_settings = dynamic_cast< VolumeChangeControlCueSettings* >( m_settings.data() );
		return volume_change_settings->fade_time;
	}
	return QTime();
}

void ControlCueModelItem::readSettings( const QJsonObject& settings )
{
	CueModelItem::readSettings( settings );
	m_settings->readSettings( settings["control-settings"].toObject() );

	if ( m_settings->cue_action == ControlAction_VolumeChange )
	{
		m_settings.reset( new VolumeChangeControlCueSettings );
		m_settings->readSettings( settings["control-settings"].toObject() );

		updatePlayer();
	}
}

void ControlCueModelItem::writeSettings( QJsonObject& settings ) const
{
	CueModelItem::writeSettings( settings );

	QJsonObject controlSettings;
	m_settings->writeSettings( controlSettings );
	settings["control-settings"] = controlSettings;
}

void ControlCueModelItem::updatePlayer()
{
	if ( m_settings->cue_action == ControlAction_VolumeChange )
	{
		playerTimeChanged( QTime( 0, 0, 0 ) );
	}
}

void ControlCueModelItem::playerTimeChanged( const QTime& time )
{
	QTime remaining;
	QTime duration = getDuration();
	if ( utils::isZero( duration ) )
	{
		remaining = QTime( 0, 0, 0 );
	}
	else
	{
		remaining = utils::subtract( duration, time );
	}

	setData( Column_Remaining, remaining );
	setData( Column_Elapsed, time );

	Q_EMIT dataChanged( this );
}

void ControlCueModelItem::fadeDone()
{
	if ( m_fade_player )
	{
		Application::getAudioManager().releasePlayer( m_fade_player );
		m_fade_player.clear();
	}

	// Stop target cue if requested
	if ( m_settings->cue_action == ControlAction_VolumeChange )
	{
		auto volume_change_settings = dynamic_cast< VolumeChangeControlCueSettings* >( m_settings.data() );
		if ( volume_change_settings->stop_target_on_end )
		{
			CueModelItem* parent = this->parent();
			CueModelItem* target = 0;
			for ( int i = 0; i != parent->row( this ); ++i )
			{
				CueModelItem* item = parent->child( i );
				if ( item->getUuid() == m_settings->target_cue_uuid )
				{
					target = item;
				}
			}

			if ( target && target->getType() == CueType_Audio )
			{
				AudioCueModelItem* audio_cue = ( AudioCueModelItem* )target;
				audio_cue->getPlayer()->stop();
			}
		}
	}
}

} /* namespace noises */

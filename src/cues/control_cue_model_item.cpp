
#include <QPixmap>

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
		case ControlAction_ITEM_COUNT:
			return "";
	}
	return "";
}

// ControlCueSettings

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

// ControlCueModelItem

ControlCueModelItem::ControlCueModelItem(
	const QList< QVariant >& item_data,
	CueModelItem* parent_item )
:
		CueModelItem( item_data, parent_item )
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
		if ( item->getUuid() == m_settings.target_cue_uuid )
		{
			target = item;
		}
	}

	if ( target && target->getType() == CueType_Audio )
	{
		AudioCueModelItem* audio_cue = ( AudioCueModelItem* )target;

		switch( m_settings.cue_action )
		{
			case ControlAction_Start:
				audio_cue->start();
				break;
			case ControlAction_Stop:
				audio_cue->stop();
				break;
			case ControlAction_Pause:
				audio_cue->pause();
				break;
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

void ControlCueModelItem::readSettings( const QJsonObject& settings )
{
	CueModelItem::readSettings( settings );
	m_settings.readSettings( settings["control-settings"].toObject() );
}

void ControlCueModelItem::writeSettings( QJsonObject& settings ) const
{
	CueModelItem::writeSettings( settings );

	QJsonObject controlSettings;
	m_settings.writeSettings( controlSettings );
	settings["control-settings"] = controlSettings;
}

} /* namespace noises */

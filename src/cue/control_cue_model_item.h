#pragma once

#include "cue_model_item.h"

namespace noises
{
	enum ControlAction
	{
		ControlAction_Stop,
		ControlAction_Start,
		ControlAction_Pause,
		ControlAction_VolumeChange,

		ControlAction_ITEM_COUNT
	};

	inline ControlAction& operator++( ControlAction& action )
	{
		action = ( ControlAction )( ( int )action + 1 );
		return action;
	}

	QString actionToString( ControlAction action );

	// ControlCueSettings

	struct ControlCueSettings
	{
	public:
		void readSettings( const QJsonObject& root );

		void writeSettings( QJsonObject& root ) const;

	public:
		QString target_cue_uuid;
		ControlAction cue_action;
	};

	// ControlCueModelItem

	class ControlCueModelItem: public CueModelItem
	{
	public:
		ControlCueModelItem(
			const QList< QVariant >& item_data,
			CueModelItem* parent_item = nullptr );

		~ControlCueModelItem();

		CueType getType() const override
		{
			return CueType_Control;
		}

		ControlCueSettings& getSettings()
		{
			return m_settings;
		}

		void execute() override;

		void readSettings( const QJsonObject& settings ) override;

		void writeSettings( QJsonObject& settings ) const override;

	protected:
		QVariant getIcon() const override;

	private:
		ControlCueSettings m_settings;
	};

} /* namespace noises */

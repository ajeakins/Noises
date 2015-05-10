#pragma once

#include <audio/volume_matrix.h>

#include <utils/macro.h>

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

	NOISES_DECLARE_ITERABLE_ENUM( ControlAction )

	QString actionToString( ControlAction action );

	// ControlCueSettings

	class ControlCueSettings
	{
	public:
		typedef QScopedPointer< ControlCueSettings > Ptr;

		virtual ~ControlCueSettings();

		virtual void readSettings( const QJsonObject& value );

		virtual void writeSettings( QJsonObject& value ) const;

	public:
		QString target_cue_uuid;
		ControlAction cue_action;
	};

	// VolumeChangeControlCueSettings

	class VolumeChangeControlCueSettings: public ControlCueSettings
	{
	public:
		typedef QScopedPointer< VolumeChangeControlCueSettings > Ptr;

		~VolumeChangeControlCueSettings();

		void readSettings( const QJsonObject& value ) override;

		void writeSettings( QJsonObject& value ) const override;

	public:
		QTime fade_time;
		bool stop_target_on_end;
		audio::VolumeMatrix target_levels;
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

		bool hasDuration() const override;

		QString getTimeFormat() const override;

		QTime getDuration() const override;

		ControlCueSettings::Ptr& getSettings()
		{
			return m_settings;
		}

		void execute() override;

		void readSettings( const QJsonObject& settings ) override;

		void writeSettings( QJsonObject& settings ) const override;

		void updatePlayer();

	protected:
		QVariant getIcon() const override;

	private Q_SLOTS:
		void playerTimeChanged( const QTime& time );

	private:
		ControlCueSettings::Ptr m_settings;
	};

} /* namespace noises */

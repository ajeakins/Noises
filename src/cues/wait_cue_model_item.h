#pragma once

#include <QTime>

#include <audio/wait_player.h>

#include "cue_model_item.h"

namespace noises
{
	struct WaitCueSettings
	{
	public:
		void readSettings( const QJsonObject& root );

		void writeSettings( QJsonObject& root ) const;

	public:
		QTime wait_time;
	};

	class WaitCueModelItem: public CueModelItem
	{
	public:
		WaitCueModelItem(
			const QList< QVariant >& item_data,
			CueModelItem* parent_item = nullptr );

		~WaitCueModelItem();

		CueType getType() const
		{
			return CueType_Wait;
		}

		PostActions getSupportedPostActions()
		{
			return PostActions( PostAction_AdvanceAndPlay );
		}

		WaitCueSettings& getSettings()
		{
			return m_settings;
		}

		void execute() const;

		void readSettings( const QJsonObject& settings );

		void writeSettings( QJsonObject& settings ) const;

		// get rid of this...
		void updatePlayer();

	protected:
		QVariant getIcon() const;

	private Q_SLOTS:
		void playerTimeChanged( const QTime& time );

	private:
		WaitCueSettings m_settings;

		audio::WaitPlayer::Ptr m_player;
	};

} /* namespace noises */

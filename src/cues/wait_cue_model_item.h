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

		CueType getType() const override
		{
			return CueType_Wait;
		}

		PostActions getSupportedPostActions() const override
		{
			return PostActions( PostAction_AdvanceAndPlay );
		}

		WaitCueSettings& getSettings()
		{
			return m_settings;
		}

		void execute() override;

		void readSettings( const QJsonObject& settings ) override;

		void writeSettings( QJsonObject& settings ) const override;

		const QString& getTimeFormat() const
		{
			return m_time_format;
		}

		const QTime& getDuration() const
		{
			return m_settings.wait_time;
		}

		// get rid of this...
		void updatePlayer();

	protected:
		QVariant getIcon() const override;

	private Q_SLOTS:
		void playerTimeChanged( const QTime& time );

		void waitDone();

	private:
		WaitCueSettings m_settings;

		QString m_time_format;

		audio::WaitPlayer::Ptr m_player;
	};

} /* namespace noises */

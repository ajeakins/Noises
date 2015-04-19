#pragma once

#include <assert.h>

#include <QList>
#include <QVariant>

#include <QJsonObject>

#include "types.h"

namespace noises
{
	class CueModelItem: public QObject
	{
		Q_OBJECT
	public:
		CueModelItem(
			const QList< QVariant >& item_data,
			CueModelItem* parent_item = nullptr );

		virtual ~CueModelItem();

		CueModelItem* parent();
		CueModelItem* child( int row );

		int childCount() const;
		int columnCount() const;

		void appendChild( CueModelItem* child );
		void insertChild( int row, CueModelItem* child );

		int row( CueModelItem* child );

		void deleteChild( CueModelItem* child );
		void deleteChild( int row );

		QVariant data( int column, int role = Qt::DisplayRole ) const;
		bool setData( int column, const QVariant& data );

		virtual Qt::ItemFlags flags() const;

		int row() const;

		// type of the cue
		virtual CueType getType() const
		{
			return CueType_None;
		}

		virtual PostActions getSupportedPostActions() const
		{
			return PostActions( PostAction_Advance | PostAction_AdvanceAndPlay );
		}

		QString getUuid() const
		{
			return m_uuid;
		}

		// execute the cues action
		virtual void execute()
		{
			assert( false && "Cannot execute base item" );
		}

		virtual void readSettings( const QJsonObject& settings );

		virtual void writeSettings( QJsonObject& settings ) const;

	Q_SIGNALS:
		void dataChanged( CueModelItem* item );

		void cueDone( CueModelItem* item );

	protected:
		virtual QVariant getIcon() const;

	private:
		typedef QList< QVariant > DataList;
		DataList m_item_data;

		CueModelItem* m_parent_item;
		QList< CueModelItem* > m_child_items;

		QString m_uuid;
	};

} /* namespace noises */

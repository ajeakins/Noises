#pragma once

#include <assert.h>

#include <QList>
#include <QVariant>

#include <json/json.h>

#include "types.h"

namespace noises
{
	class CueModelItem: public QObject
	{
		Q_OBJECT
	public:
		CueModelItem( const QList< QVariant >& item_data, CueModelItem* parent_item = 0 );

		virtual ~CueModelItem();

		CueModelItem* parent();
		CueModelItem* child( int row );

		int childCount() const;
		int columnCount() const;

		void appendChild( CueModelItem* child );
		void insertChild( int row, CueModelItem* child );

		int row( CueModelItem* child );

		QVariant data( int column, int role = Qt::DisplayRole ) const;
		bool setData( int column, const QVariant& data );

		virtual Qt::ItemFlags flags() const;

		int row() const;

		// type of the cue
		virtual CueType getType() const
		{
			return CueType_None;
		}

		// execute the cues action
		virtual void execute() const
		{
			assert( false && "Cannot execute base item" );
		}

		virtual void readSettings( const Json::Value& root );

		virtual void writeSettings( Json::Value& root ) const;

	protected:
		virtual QVariant getIcon() const;

	private:
		typedef QList< QVariant > DataList;
		DataList m_item_data;

		CueModelItem* m_parent_item;
		QList< CueModelItem* > m_child_items;
	};

} /* namespace noises */
#include <assert.h>
#include <iostream>

#include <QStringList>

#include "cue_model_item.h"

namespace noises
{

CueModelItem::CueModelItem( const QList< QVariant >& item_data, CueModelItem* parent_item )
:
	QObject(),
	m_item_data( item_data ),
	m_parent_item( parent_item )

{
	assert( item_data.count() == columnCount() );
}

CueModelItem::~CueModelItem()
{
	qDeleteAll( m_child_items );
}

void CueModelItem::appendChild( CueModelItem* item )
{
	m_child_items.append(item);
}

void CueModelItem::insertChild( int row, CueModelItem* item )
{
	m_child_items.insert( row, item );
}

void CueModelItem::deleteChild( CueModelItem* item )
{
	m_child_items.removeOne( item );
	item->deleteLater();
}

int CueModelItem::row( CueModelItem* item )
{
	return m_child_items.indexOf( item );
}

CueModelItem* CueModelItem::child( int row )
{
	assert( row  < childCount() );

	return m_child_items.value( row );
}

int CueModelItem::childCount() const
{
	return m_child_items.count();
}

int CueModelItem::columnCount() const
{
	return m_item_data.count();
}

QVariant CueModelItem::data( int column, int role ) const
{
	assert( column < columnCount() );

	if ( role == Qt::DisplayRole )
	{
		return m_item_data.value( column );
	}
	else if ( role == Qt::DecorationRole && column == 0 )
	{
		return getIcon();
	}

	return QVariant();
}

bool CueModelItem::setData( int column, const QVariant& data )
{
	assert( column < m_item_data.count() );
	m_item_data[ column ] = data;

	return true;
}

Qt::ItemFlags CueModelItem::flags() const
{
	Qt::ItemFlags flags =
		Qt::ItemIsEnabled |
		Qt::ItemIsSelectable |
		Qt::ItemIsDragEnabled;
	return flags;
}

CueModelItem* CueModelItem::parent()
{
	return m_parent_item;
}

int CueModelItem::row() const
{
	if ( m_parent_item )
	{
		return m_parent_item->m_child_items.indexOf( const_cast< CueModelItem* >( this ) );
	}

	return 0;
}

QVariant CueModelItem::getIcon() const
{
	return QVariant();
}

void CueModelItem::readSettings( const Json::Value& root )
{
	assert( root.type() == Json::objectValue );

	for ( int i = 0; i != m_item_data.count(); ++i )
	{
		m_item_data[i] = root["data"][i].asCString();
	}
}

void CueModelItem::writeSettings( Json::Value& root ) const
{
	assert( root.type() == Json::objectValue );

	root["type"] = qPrintable( typeToString( getType() ) );

	Json::Value values( Json::arrayValue );
	for ( auto itr = m_item_data.begin(); itr != m_item_data.end(); ++itr )
	{
		Json::Value value( qPrintable( itr->toString() ) );
		values.append( value );
	}

	root["data"] = values;
}

} /* namespace noises */
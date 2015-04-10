
#include <assert.h>

#include <QStringList>

#include <QJsonArray>

#include <cue_widget/types.h>

#include "cue_model_item.h"

namespace noises
{

CueModelItem::CueModelItem(
	const QList< QVariant >& item_data,
	CueModelItem* parent_item )
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
	item->m_parent_item = this;
}

void CueModelItem::insertChild( int row, CueModelItem* item )
{
	m_child_items.insert( row, item );
	item->m_parent_item = this;
}

void CueModelItem::deleteChild( CueModelItem* item )
{
	m_child_items.removeOne( item );
	item->deleteLater();
}

void CueModelItem::deleteChild( int row )
{
	assert( row < m_child_items.size() );

	CueModelItem* item = m_child_items.takeAt( row );
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

	// enable drop on the root
	if ( getType() == CueType_None )
	{
		flags |= Qt::ItemIsDropEnabled;
	}

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

void CueModelItem::readSettings( const QJsonObject& settings )
{
	QJsonArray data = settings.value( "data" ).toArray();

	for ( int i = 0; i != m_item_data.count(); ++i )
	{
		m_item_data[i] = data[i].toString();
	}
}

void CueModelItem::writeSettings( QJsonObject& settings ) const
{
	settings.insert( "type", typeToString( getType() ) );

	QJsonArray values;
	for ( auto itr = m_item_data.begin(); itr != m_item_data.end(); ++itr )
	{
		QJsonValue value( itr->toString() );
		values.append( value );
	}

	settings["data"] = values;
}

} /* namespace noises */

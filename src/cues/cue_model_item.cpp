#include <assert.h>

#include <QStringList>

#include "cue_model_item.h"

namespace noises
{

CueModelItem::CueModelItem( const QList< QVariant >& item_data, CueModelItem* parent_item )
:
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

QVariant CueModelItem::data( int column ) const
{
	assert( column < columnCount() );

	return m_item_data.value( column );
}

bool CueModelItem::setData( int column, const QVariant& data )
{
	assert( column < m_item_data.count() );
	m_item_data[ column ] = data;

	return true;
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

} /* namespace noises */
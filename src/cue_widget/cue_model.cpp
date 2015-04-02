#include <assert.h>

#include <QtGui>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <cues/audio_cue_model_item.h>
#include <cues/control_cue_model_item.h>
#include <cues/group_cue_model_item.h>
#include <cues/wait_cue_model_item.h>

#include "cue_model.h"
#include "types.h"

namespace noises
{

CueModel::CueModel( QObject* parent )
:
	QAbstractItemModel( parent )
{
	// Setup header item
	QList< QVariant > rootData;
	for ( Column itr = ( Column )0; itr != ColumnCount; ++itr )
	{
		rootData.append( columnToString( itr ) );
	}
	m_root_item = new CueModelItem( rootData );
}

CueModel::~CueModel()
{
	delete m_root_item;
}

int CueModel::columnCount( const QModelIndex& parent ) const
{
	if ( parent.isValid() )
	{
		return static_cast< CueModelItem* >( parent.internalPointer() )->columnCount();
	}
	else
	{
		return m_root_item->columnCount();
	}
}

QVariant CueModel::data( const QModelIndex& index, int role ) const
{
	if ( !index.isValid() )
	{
		return QVariant();
	}

	CueModelItem *item = static_cast< CueModelItem* >( index.internalPointer() );
	return item->data( index.column(), role );
}

CueModelItem* CueModel::itemFromIndex( const QModelIndex& index ) const
{
	if (index.isValid())
	{
		CueModelItem* item = static_cast< CueModelItem* >( index.internalPointer() );
		if ( item )
		{
			return item;
		}
	}
	return m_root_item;
}

bool CueModel::setData(
	const QModelIndex& index,
	const QVariant &value,
	int role)
{
	if ( role != Qt::EditRole)
	{
		return false;
	}

	CueModelItem *item = itemFromIndex( index );
	bool result = item->setData( index.column(), value );

	if ( result )
	{
		Q_EMIT dataChanged( index, index );
	}

	return result;
}

CueModelItem* CueModel::createCue( CueType type )
{
	assert( m_root_item );
	assert( type != CueType_None );

	// fill data
	QList< QVariant > data;

	for ( Column itr = ( Column )0; itr != ColumnCount; ++itr )
	{
		data.append( "" );
	}
	data[Column_PostAction] = postActionToString( defaultPostAction() );

	CueModelItem* item = 0;

	switch( type )
	{
		case CueType_Audio:
			item = new AudioCueModelItem( data, m_root_item );
			break;
		case CueType_Control:
			item = new ControlCueModelItem( data, m_root_item );
			break;
		case CueType_Wait:
			item = new WaitCueModelItem( data, m_root_item );
			break;
		case CueType_Group:
			item = new GroupCueModelItem( data, m_root_item );
			break;
		default:
			assert( false && "Unhandled cue type" );
			break;
	}

	connect(
		item, &CueModelItem::dataChanged,
		this, &CueModel::itemDataChanged
		);

	return item;
}

bool CueModel::removeRows( int row, int count, const QModelIndex &parent )
{
	CueModelItem* parent_item = parent.isValid() ? itemFromIndex( parent ) : m_root_item;

	// check that we can remove the specified rows
	if ( (row + count) > parent_item->childCount() )
	{
		return false;
	}

	beginRemoveRows( parent, row, row + count );
	for ( int i = row + count; i != row; --i )
	{
		parent_item->deleteChild( i - 1 );
	}
	endRemoveRows();

	return true;
}

void CueModel::setCueParent( CueModelItem* parent, CueModelItem* child, int row )
{
	Q_EMIT  layoutAboutToBeChanged();

	parent->insertChild( row, child );

	Q_EMIT layoutChanged();
}

Qt::ItemFlags CueModel::flags( const QModelIndex& index ) const
{
	if ( !index.isValid() )
	{
		return 0;
	}

	CueModelItem *item = static_cast< CueModelItem* >( index.internalPointer() );
	return item->flags();
}

QVariant CueModel::headerData(
	int section,
	Qt::Orientation orientation,
	int role ) const
{
	if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
	{
		return m_root_item->data( section );
	}

	return QVariant();
}

QModelIndex CueModel::index(
	int row,
	int column,
	const QModelIndex& parent ) const
{
	if ( !hasIndex( row, column, parent ) )
	{
		return QModelIndex();
	}

	CueModelItem* parentItem;

	if ( !parent.isValid() )
	{
		parentItem = m_root_item;
	}
	else
	{
		parentItem = static_cast< CueModelItem* >( parent.internalPointer() );
	}

	CueModelItem* childItem = parentItem->child( row );
	if ( childItem )
	{
		return createIndex( row, column, childItem );
	}
	else
	{
		return QModelIndex();
	}
}

QModelIndex CueModel::parent( const QModelIndex& index ) const
{
	if ( !index.isValid() )
	{
		return QModelIndex();
	}

	CueModelItem* childItem = static_cast< CueModelItem* >( index.internalPointer() );
	CueModelItem* parentItem = childItem->parent();

	if ( parentItem == m_root_item )
	{
		return QModelIndex();
	}

	return createIndex( parentItem->row(), 0, parentItem );
}

int CueModel::rowCount( const QModelIndex& parent ) const
{
	CueModelItem* parentItem;
	if (parent.column() > 0)
	{
		return 0;
	}

	if (!parent.isValid())
	{
		parentItem = m_root_item;
	}
	else
	{
		parentItem = static_cast< CueModelItem* >( parent.internalPointer() );
	}

	return parentItem->childCount();
}

Qt::DropActions CueModel::supportedDropActions() const
{
	return Qt::MoveAction;
}

QStringList CueModel::mimeTypes() const
{
	QStringList types;
	types << "application/noises.text.cues";
	return types;
}

QMimeData* CueModel::mimeData( const QModelIndexList& indexes ) const
{
	QJsonArray data;

	for ( auto itr = indexes.begin(); itr != indexes.end(); ++itr )
	{
		QModelIndex index = *itr;

		// Erm shouldn't need this... Get rid of it :-(
		if( index.column() != 0 )
		{
			continue;
		}

		if ( index.isValid() )
		{
			CueModelItem* item = itemFromIndex( index );
			QJsonObject cueData;

			item->writeSettings( cueData );
			data.push_back( cueData );
		}
	}

	QJsonDocument document( data );

	QMimeData *mimeData = new QMimeData();
	mimeData->setData( "application/noises.text.cues", document.toJson() );

	return mimeData;
}

bool CueModel::dropMimeData(
	const QMimeData* data,
	Qt::DropAction action,
	int /*row*/,
	int /*column*/,
	const QModelIndex& parent_index )
{
	if ( action == Qt::IgnoreAction )
	{
		return true;
	}

	if ( !data->hasFormat( "application/noises.text.cues" ) )
	{
		return false;
	}

	QByteArray encodedData = data->data( "application/noises.text.cues" );
	QJsonDocument document = QJsonDocument::fromJson( encodedData );
	const QJsonArray cues = document.array();

	CueModelItem* parent = itemFromIndex( parent_index );

	for( auto itr = cues.begin(); itr != cues.end(); ++itr )
	{
		QJsonObject cueData = itr->toObject();

		QString type = cueData["type"].toString(); // error handling

		CueModelItem* item = createCue( stringToType( type ) );
		setCueParent( parent, item, parent->childCount() + 1 );

		item->readSettings( cueData );
	}

    return true;
}

void CueModel::itemDataChanged( CueModelItem* item )
{
	// TODO need to compute row relative to parent
	int row = item->parent()->row( item );
	QModelIndex from = this->index( row, 0 );
	QModelIndex to = this->index( row, item->columnCount() );
	Q_EMIT dataChanged( from, to );
}

void CueModel::readSettings( const QJsonArray& settings )
{
	for( auto itr = settings.begin(); itr != settings.end(); ++itr )
	{
		QJsonValue value = *itr;
		if ( !value.isObject() )
		{
			continue; // warn here...
		}

		QJsonObject cueData = value.toObject();
		QString type = cueData["type"].toString();

		CueModelItem* item = createCue( stringToType( type ) );
		setCueParent( m_root_item, item, m_root_item->childCount() + 1 );

		item->readSettings( cueData );
	}
}

void CueModel::writeSettings( QJsonArray& settings ) const
{
	for ( int i = 0; i != m_root_item->childCount(); ++i )
	{
		QJsonObject cueData;
		m_root_item->child( i )->writeSettings( cueData );
		settings.push_back( cueData );
	}
}

} /* namespace noises */

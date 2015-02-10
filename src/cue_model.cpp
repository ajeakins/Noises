#include <assert.h>

#include <QtGui>

#include <cues/audio_cue_model_item.h>
#include <cues/control_cue_model_item.h>
#include <cues/group_cue_model_item.h>
#include <cues/wait_cue_model_item.h>

#include "cue_model.h"

namespace noises
{

CueModel::CueModel( QObject* parent )
:
	QAbstractItemModel( parent )
{
	// Setup header item
	QList< QVariant > rootData;
	rootData << "Cue" << "Description" << "Notes" << "Remaining" << "Elapsed";

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
	for ( int i = 0; i != columnCount(); ++i )
	{
		data.push_back( "" );
	}

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
	qDebug() << indexes.size();

	Json::Value data( Json::arrayValue );

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

			Json::Value cueData( Json::objectValue );
			item->writeSettings( cueData );
			data.append( cueData );

			qDebug() << "append";
		}
	}

	Json::StyledWriter writer;
	std::string dataString = writer.write( data );

	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData( dataString.c_str() );

	mimeData->setData( "application/noises.text.cues", encodedData );
	return mimeData;
}

bool CueModel::dropMimeData(
	const QMimeData* data,
	Qt::DropAction action,
	int row,
	int column,
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

	CueModelItem* parent = itemFromIndex( parent_index );

	QByteArray encodedData = data->data( "application/noises.text.cues" );
	std::string decodedData( encodedData.data() );

	Json::Value root;

	Json::Reader reader;
	reader.parse( decodedData, root );

	assert( root.type() == Json::arrayValue );

	for ( unsigned int i = 0; i != root.size(); ++i )
	{
		Json::Value cueData = root[i];
		assert( cueData.type() == Json::objectValue );

		QString type = cueData["type"].asCString();

		CueModelItem* item = createCue( stringToType( type ) );
		setCueParent( parent, item, parent->childCount() + 1 );

		item->readSettings( cueData );
	}

    return true;
 }

void CueModel::readSettings( const Json::Value& root )
{
	assert( root.type() == Json::arrayValue );

	for ( unsigned int i = 0; i != root.size(); ++i )
	{
		Json::Value cueData = root[i];
		assert( cueData.type() == Json::objectValue );

		QString type = cueData["type"].asCString();

		CueModelItem* item = createCue( stringToType( type ) );
		setCueParent( m_root_item, item, m_root_item->childCount() + 1 );

		item->readSettings( cueData );
	}
}

void CueModel::writeSettings( Json::Value& root ) const
{
	assert( root.type() == Json::arrayValue );

	for ( int i = 0; i != m_root_item->childCount(); ++i )
	{
		Json::Value cueData( Json::objectValue );
		m_root_item->child( i )->writeSettings( cueData );
		root.append( cueData );
	}
}

} /* namespace noises */

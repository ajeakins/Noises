#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QObject>

#include "types.h"

namespace noises
{
	class CueModelItem;

	class CueModel : public QAbstractItemModel
	{
		Q_OBJECT

	public:
		CueModel( QObject *parent = 0 );
		~CueModel();

		QVariant data( const QModelIndex& index, int role ) const;
		bool setData(
			const QModelIndex &index,
			const QVariant &value,
			int role = Qt::EditRole);

		Qt::ItemFlags flags( const QModelIndex& index ) const;
		QVariant headerData(
			int section,
			Qt::Orientation orientation,
			int role = Qt::DisplayRole ) const;

		QModelIndex index(
			int row,
			int column,
			const QModelIndex& parent = QModelIndex() ) const;

		CueModelItem* itemFromIndex( const QModelIndex& index ) const;

		QModelIndex parent( const QModelIndex& index ) const;

		CueModelItem* createCue( CueType type );

		void setCueParent( CueModelItem* parent, CueModelItem* child, int row );

		int rowCount( const QModelIndex& parent = QModelIndex() ) const;
		int columnCount( const QModelIndex& parent = QModelIndex() ) const;

		CueModelItem* getRootItem()
		{
			return m_root_item;
		}

	private:
		CueModelItem* m_root_item;
	};

} /* namespace noises */
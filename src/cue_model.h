#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QObject>

#include <QJsonArray>

#include <cues/types.h>

namespace noises
{
	// TODO: propagate this and couple it with the actual data...
	// also might not belong here really...
	enum Column
	{
		Column_Cue,
		Column_Description,
		Column_Notes,
		Column_Remaining,
		Column_Elapsed,
		Column_PostAction
	};

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

		bool removeRows( int row, int count, const QModelIndex &parent = QModelIndex() );

		void setCueParent( CueModelItem* parent, CueModelItem* child, int row );

		int rowCount( const QModelIndex& parent = QModelIndex() ) const;
		int columnCount( const QModelIndex& parent = QModelIndex() ) const;

		CueModelItem* getRootItem()
		{
			return m_root_item;
		}

		Qt::DropActions supportedDropActions() const;

		QStringList mimeTypes() const;

		QMimeData *mimeData( const QModelIndexList& indexes ) const;

		bool dropMimeData(
			const QMimeData* data,
			Qt::DropAction action,
			int row,
			int column,
			const QModelIndex& parent );

		void readSettings( const QJsonArray& settings );

		void writeSettings( QJsonArray& settings ) const;

	private Q_SLOTS:
		void itemDataChanged( CueModelItem* item );

	private:
		CueModelItem* m_root_item;
	};

} /* namespace noises */
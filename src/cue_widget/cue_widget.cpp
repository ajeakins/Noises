
#include <QAction>
#include <QContextMenuEvent>
#include <QDataWidgetMapper>
#include <QMenu>
#include <QMessageBox>

#include <cue_dialogs/api.h>

#include <cue/cue_model_item.h>

#include <utils/cue.h>

#include "cue_model.h"
#include "progress_delegate.h"
#include "types.h"

#include "cue_widget.h"

namespace noises
{

CueWidget::CueWidget( QWidget* parent )
:
	QTreeView( parent )
{
	m_cue_model = new CueModel;

	setModel( m_cue_model );

	setSelectionMode( QAbstractItemView::SingleSelection );
	setSelectionBehavior( QAbstractItemView::SelectRows );
	setDragEnabled( true );
	setDragDropMode( QAbstractItemView::InternalMove );
	setAcceptDrops( true );
	setDropIndicatorShown( true );

	ProgressDelegate* progress_delegate = new ProgressDelegate( this );
	setItemDelegateForColumn( Column_Remaining, progress_delegate );
	setItemDelegateForColumn( Column_Elapsed, progress_delegate );

	connect(
		this, SIGNAL( doubleClicked( QModelIndex ) ),
		this, SLOT( editCue( QModelIndex ) ) );

	connect(
		m_cue_model, &CueModel::cueDone,
		this, &CueWidget::cueDone );

	m_edit_cue_action = new QAction( QIcon( ":/images/edit_16x16.png" ), "Edit Cue", this );
	connect( m_edit_cue_action, &QAction::triggered,
		this, &CueWidget::editCurrentCue );

	m_delete_cue_action = new QAction( QIcon( ":/images/delete_16x16.png" ), "Delete Cue", this );
	connect( m_delete_cue_action, &QAction::triggered,
		this, &CueWidget::deleteCurrentCue );
}

CueModelItem* CueWidget::createCue( CueType type )
{
	QModelIndex selected_row = selectionModel()->currentIndex();

	CueModelItem* parent;
	int index = 0;
	QString previous_number, next_number, new_number;

	if ( selected_row.isValid() )
	{
		CueModelItem* selected_item = ( CueModelItem* )( selected_row.internalPointer() );
		parent = selected_item->parent();
		index = parent->row( selected_item );
		previous_number = selected_item->data( 0 ).toString();
	}
	else
	{
		parent = m_cue_model->getRootItem();
		index = parent->childCount();
	}

	QModelIndex next_row = m_cue_model->index( selected_row.row() + 1, selected_row.column() );
	if ( next_row.isValid() )
	{
		CueModelItem* next_row_item = ( CueModelItem* )( next_row.internalPointer() );
		next_number = next_row_item->data( 0 ).toString();
	}

	utils::getNewCueNumber( previous_number, next_number, new_number );

	// add the cue
	CueModelItem* cue = m_cue_model->createCue( type );
	cue->setData( 0, new_number );

	m_cue_model->setCueParent( parent, cue, index + 1 );

	// update the selection
	if ( ! selected_row.isValid() )
	{
		selected_row = m_cue_model->index( 0, 0 );
		selectionModel()->setCurrentIndex(
			selected_row,
			QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
	}
	else
	{
		selected_row = m_cue_model->index( selected_row.row() + 1, selected_row.column() );
		selectionModel()->setCurrentIndex(
			selected_row,
			QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
	}

	Q_EMIT cueSettingChanged();
	return cue;
}

void CueWidget::deleteCurrentCue()
{
	QMessageBox message(
		QMessageBox::Question,
		"Delete Cue",
		"Are you sure you want to delete?",
		QMessageBox::Ok | QMessageBox::Cancel,
		this );
	int ret = message.exec();

	if ( ret != QMessageBox::Ok )
	{
		return;
	}

	QModelIndex selected_row = selectionModel()->currentIndex();
	if ( !selected_row.isValid() )
	{
		return;
	}

	m_cue_model->removeRow( selected_row.row(), selected_row.parent() );
	Q_EMIT cueSettingChanged();
}

void CueWidget::editCue( QModelIndex index )
{
	CueModelItem* item = m_cue_model->itemFromIndex( index );
	if( showCueEditDialog( item, getDataMapperForSelection(), false, this ) )
	{
		Q_EMIT cueSettingChanged();
	}
}

void CueWidget::editCurrentCue()
{
	QModelIndex selected_row = selectionModel()->currentIndex();
	if ( !selected_row.isValid() )
	{
		return;
	}

	editCue( selected_row );
}

void CueWidget::cueDone( CueModelItem* /*item*/ )
{
	// TODO: add some checking this is still the selected cue

	QModelIndex selected_row = selectionModel()->currentIndex();
	selected_row = m_cue_model->index( selected_row.row() + 1, selected_row.column() );
	selectionModel()->setCurrentIndex(
		selected_row,
		QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
}

CueModelItem* CueWidget::getCurrentItem()
{
	QModelIndex selected_row = selectionModel()->currentIndex();
	if ( !selected_row.isValid() )
	{
		return nullptr;
	}

	return m_cue_model->itemFromIndex( selected_row );
}

void CueWidget::selectNextCue()
{
	QModelIndex selected_row = selectionModel()->currentIndex();
	if ( !selected_row.isValid() )
	{
		return;
	}

	selected_row = m_cue_model->index( selected_row.row() + 1, selected_row.column() );
	selectionModel()->setCurrentIndex(
		selected_row,
		QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
}

QDataWidgetMapper* CueWidget::getDataMapperForSelection()
{
	QDataWidgetMapper* mapper = new QDataWidgetMapper();
	mapper->setModel( m_cue_model );
	mapper->setSubmitPolicy( QDataWidgetMapper::ManualSubmit );
	mapper->setCurrentModelIndex( selectionModel()->currentIndex() );
	return mapper;
}

void CueWidget::contextMenuEvent( QContextMenuEvent* event )
{
	QMenu menu;
	menu.addAction( m_edit_cue_action );
	menu.addSeparator();
	menu.addAction( m_delete_cue_action );
	menu.exec( event->globalPos() );
}

void CueWidget::readSettings( const QJsonArray& settings )
{
	m_cue_model->readSettings( settings );
}

void CueWidget::writeSettings( QJsonArray& settings ) const
{
	m_cue_model->writeSettings( settings );
}

} /* namespace noises */

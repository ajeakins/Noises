#include <assert.h>
#include <iostream>

#include <QtGui>
#include <QDataWidgetMapper>

#include <utils/cue.h>
#include <cue_dialogs/audio_cue_dialog.h>

#include "main_window.h"
#include "cue_model.h"

namespace noises
{

MainWindow::MainWindow()
{
	m_cue_model = new CueModel;

	m_cue_list = new QTreeView;
	m_cue_list->setModel( m_cue_model );

	m_cue_list->setSelectionMode( QAbstractItemView::SingleSelection );
	m_cue_list->setDragEnabled( true );
	m_cue_list->setAcceptDrops( true );
	m_cue_list->setDropIndicatorShown( true );

	connect(
		m_cue_list, SIGNAL( doubleClicked( QModelIndex ) ),
		this, SLOT( editCue( QModelIndex ) ) );

	setCentralWidget( m_cue_list );

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	m_cue_list->addAction( m_play_cue_action );

	resize( 600, 500 );
}

QDataWidgetMapper* MainWindow::getDataMapper()
{
	QDataWidgetMapper* mapper = new QDataWidgetMapper();
	mapper->setModel( m_cue_model );
	mapper->setSubmitPolicy( QDataWidgetMapper::ManualSubmit );
	mapper->setCurrentModelIndex( m_cue_list->selectionModel()->currentIndex() );
	return mapper;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
	// check state and confirm
	event->accept();
}

void MainWindow::newShow()
{
	std::cout << "newShow" << std::endl;
}

void MainWindow::newAudioCue()
{
	QModelIndex selected_row = m_cue_list->selectionModel()->currentIndex();

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
	}

	QModelIndex next_row = m_cue_model->index( selected_row.row() + 1, selected_row.column() );
	if ( next_row.isValid() )
	{
		CueModelItem* next_row_item = ( CueModelItem* )( next_row.internalPointer() );
		next_number = next_row_item->data( 0 ).toString();
	}

	utils::getNewCueNumber( previous_number, next_number, new_number );

	// add the cue

	AudioCueModelItem* cue = ( AudioCueModelItem* )( m_cue_model->createCue( CueType_Audio ) );
	cue->setData( 0, new_number );

	m_cue_model->setCueParent( parent, cue, index + 1 );

	// update the selection

	if ( ! selected_row.isValid() )
	{
		selected_row = m_cue_model->index( 0, 0 );
		m_cue_list->selectionModel()->setCurrentIndex(
			selected_row,
			QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
	}
	else
	{
		selected_row = m_cue_model->index( selected_row.row() + 1, selected_row.column() );
		m_cue_list->selectionModel()->setCurrentIndex(
			selected_row,
			QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
	}

	// open edit dialog
	AudioCueDialog dialog( cue, getDataMapper(), this );
	dialog.exec();
}

void MainWindow::about()
{
	QMessageBox::about(
		this,
		"About",
		"About message." );
}

void MainWindow::playCue()
{
	QModelIndex selected_row = m_cue_list->selectionModel()->currentIndex();
	CueModelItem* item = m_cue_model->itemFromIndex( selected_row );

	// execute cue
	if ( item )
	{
		item->execute();
	}

	// increment selection
	selected_row = m_cue_model->index( selected_row.row() + 1, selected_row.column() );
	m_cue_list->selectionModel()->setCurrentIndex(
		selected_row,
		QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
}

void MainWindow::editCue( QModelIndex index )
{
	CueModelItem* item = m_cue_model->itemFromIndex( index );
	switch( item->getType() )
	{
		case CueType_Audio:
		{
			AudioCueDialog dialog( (AudioCueModelItem*)item, getDataMapper(), this );
			dialog.exec();
			break;
		}
		case CueType_None:
			return;
		default:
			qWarning() << "WARNING: Unknow cue type\n";
			break;
	}
}

void MainWindow::createActions()
{
	// menu actions

	m_new_show_action = new QAction( "&New Show", this );
	m_new_show_action->setShortcuts( QKeySequence::New );
	m_new_show_action->setStatusTip( "Create a new show" );
	connect(
		m_new_show_action, SIGNAL( triggered() ),
		this, SLOT( newShow() ) );

	m_exit_action = new QAction( "E&xit", this );
	m_exit_action->setShortcuts( QKeySequence::Quit );
	m_exit_action->setStatusTip( "Exit the application" );
	connect(
		m_exit_action, SIGNAL( triggered() ),
		this, SLOT( close() ) );

	m_about_action = new QAction( "&About", this );
	m_about_action->setStatusTip( "About noises" );
	connect(
		m_about_action, SIGNAL(triggered()),
		this, SLOT(about()));

	m_about_qt_action = new QAction( "About &Qt", this );
	m_about_qt_action->setStatusTip( "About Qt" );
	connect(
		m_about_qt_action, SIGNAL(triggered()),
		qApp, SLOT(aboutQt()));

	// toolbar actions

	m_new_cue_action = new QAction( "&New Audio Cue", this );
	m_new_cue_action->setShortcuts( QKeySequence::New );
	m_new_cue_action->setStatusTip( "Create a new audio cue" );
	connect(
		m_new_cue_action, SIGNAL( triggered() ),
		this, SLOT( newAudioCue() ) );

	// cue actions

	m_play_cue_action = new QAction( "Play Cue", m_cue_list );
	m_play_cue_action->setShortcut( QKeySequence( Qt::Key_Space ) );
	connect(
		m_play_cue_action, SIGNAL( triggered() ),
		this, SLOT( playCue() ) );
}

void MainWindow::createMenus()
{
	m_file_menu = menuBar()->addMenu( "&File" );
	m_file_menu->addAction( m_new_show_action );
	m_file_menu->addSeparator();
	m_file_menu->addAction( m_exit_action );

	menuBar()->addSeparator();

	m_help_menu = menuBar()->addMenu( "&Help" );
	m_help_menu->addAction( m_about_action );
	m_help_menu->addAction( m_about_qt_action );
}

void MainWindow::createToolBars()
{
	m_tool_bar = addToolBar( "Tools" );
	m_tool_bar->addAction( m_new_cue_action );
}

void MainWindow::createStatusBar()
{
	statusBar()->showMessage( "Ready" );
}

} /* namespace noises */

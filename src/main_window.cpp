
#include <assert.h>
#include <iostream>

#include <QApplication>
#include <QtGui>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QFileDialog>

#include <QJsonDocument>
#include <QJsonObject>

#include <utils/cue.h>

#include <cues/audio_cue_model_item.h>
#include <cues/control_cue_model_item.h>
#include <cues/cue_model_item.h>
#include <cues/group_cue_model_item.h>
#include <cues/wait_cue_model_item.h>

#include <cue_dialogs/audio_cue_dialog.h>
#include <cue_dialogs/control_cue_dialog.h>
#include <cue_dialogs/group_cue_dialog.h>
#include <cue_dialogs/wait_cue_dialog.h>

#include "application.h"
#include "main_window.h"
#include "cue_model.h"
#include "preferences.h"
#include "progress_delegate.h"

namespace noises
{

MainWindow::MainWindow()
{
	setWindowTitle( "Noises" );

	createWidgets();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	loadSettings();

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
	saveSettings();

	event->accept();
}

void MainWindow::saveSettings()
{
	QSettings settings( "Noises", "Noises" );
}

void MainWindow::loadSettings()
{
	QSettings settings( "Noises", "Noises" );
}

void MainWindow::newShow()
{}

void MainWindow::saveShow()
{
	QString filename = QFileDialog::getSaveFileName (
		this,
		"Save Show",
		QString(),
		"*.noises" );

	// user cancelled
	if ( filename.isEmpty() )
	{
		return;
	}

	if ( !filename.endsWith( ".noises" ) )
	{
		filename += ".noises";
	}

	QJsonArray cues;
	m_cue_model->writeSettings( cues );

	QJsonObject root;
	root["cues"] = cues;

	QFile file( filename ); // scoped
	file.open( QIODevice::WriteOnly );

	QJsonDocument document( root );
	file.write( document.toJson() );
}

void MainWindow::saveShowAs()
{}

void MainWindow::openShow()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		"Open Show",
		QString(),
		"*.noises" );

	// User cancelled
	if ( filename.isEmpty() )
	{
		return;
	}

	QFile file( filename ); // scoped
	file.open( QIODevice::ReadOnly );

	QJsonDocument document = QJsonDocument::fromJson( file.readAll() );
	if ( !document.isObject() )
	{
		QMessageBox::critical( this, "Errror Loading File", "Unable to load show file." );
		return;
	}

	QJsonObject object = document.object();

	// load cues
	QJsonValue cues = object.value( "cues" );
	if ( cues.isArray() )
	{
		m_cue_model->readSettings( cues.toArray() );
	}
}

CueModelItem* MainWindow::createCue( CueType type )
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

	CueModelItem* cue = m_cue_model->createCue( type );
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

	return cue;
}

void MainWindow::newAudioCue()
{
	AudioCueModelItem* cue = dynamic_cast< AudioCueModelItem* >( createCue( CueType_Audio ) );

	AudioCueDialog dialog( cue, getDataMapper(), this );
	dialog.exec();
}

void MainWindow::newControlCue()
{
	ControlCueModelItem* cue = dynamic_cast< ControlCueModelItem* >( createCue( CueType_Control ) );

	ControlCueDialog dialog( cue, getDataMapper(), this );
	dialog.exec();
}

void MainWindow::newWaitCue()
{
	WaitCueModelItem* cue = dynamic_cast< WaitCueModelItem* >( createCue( CueType_Wait ) );

	WaitCueDialog dialog( cue, getDataMapper(), this );
	dialog.exec();
}

void MainWindow::newGroupCue()
{
	GroupCueModelItem* cue = dynamic_cast< GroupCueModelItem* >( createCue( CueType_Group ) );

	GroupCueDialog dialog( cue, getDataMapper(), this );
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
	if ( !selected_row.isValid() )
	{
		return;
	}

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
			AudioCueDialog dialog(
				( AudioCueModelItem* )item,
				getDataMapper(),
				this );
			dialog.exec();
			break;
		}
		case CueType_Control:
		{
			ControlCueDialog dialog(
				( ControlCueModelItem* )item,
				getDataMapper(),
				this );
			dialog.exec();
			break;
		}
		case CueType_Wait:
		{
			WaitCueDialog dialog(
				( WaitCueModelItem* )item,
				getDataMapper(),
				this );
			dialog.exec();
			break;
		}
		case CueType_Group:
		{
			GroupCueDialog dialog(
				( GroupCueModelItem* )item,
				getDataMapper(),
				this );
			dialog.exec();
			break;
		}
		case CueType_None:
			return;
		default:
			qWarning() << "WARNING: Unknown cue type\n";
			break;
	}
}

void MainWindow::deleteCue()
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

	QModelIndex selected_row = m_cue_list->selectionModel()->currentIndex();
	if ( !selected_row.isValid() )
	{
		return;
	}

	m_cue_model->removeRow( selected_row.row(), selected_row.parent() );
}

void MainWindow::stopAllCues()
{
	Application::getAudioManager().stop();
}

void MainWindow::editPreferences()
{
	PreferencesDialog dialog( Application::getPreferences(), this );
	dialog.exec();
}

void MainWindow::createWidgets()
{
	m_cue_model = new CueModel;

	m_cue_list = new QTreeView;
	m_cue_list->setModel( m_cue_model );

	m_cue_list->setSelectionMode( QAbstractItemView::SingleSelection );
	m_cue_list->setSelectionBehavior( QAbstractItemView::SelectRows );
	m_cue_list->setDragEnabled( true );
	m_cue_list->setDragDropMode( QAbstractItemView::InternalMove );
	m_cue_list->setAcceptDrops( true );
	m_cue_list->setDropIndicatorShown( true );

	m_progress_delegate = new ProgressDelegate();
	m_cue_list->setItemDelegateForColumn( 3, m_progress_delegate );
	m_cue_list->setItemDelegateForColumn( 4, m_progress_delegate );

	connect(
		m_cue_list, SIGNAL( doubleClicked( QModelIndex ) ),
		this, SLOT( editCue( QModelIndex ) ) );

	setCentralWidget( m_cue_list );
}

void MainWindow::createActions()
{
	// menu actions

	// file

	m_new_show_action = new QAction( "&New Show", this );
	m_new_show_action->setShortcuts( QKeySequence::New );
	m_new_show_action->setStatusTip( "Create a new show" );
	connect(
		m_new_show_action, &QAction::triggered,
		this, &MainWindow::newShow );

	m_save_show_action = new QAction( "&Save Show", this );
	m_save_show_action->setShortcuts( QKeySequence::Save );
	m_save_show_action->setStatusTip( "Save show" );
	connect(
		m_save_show_action, &QAction::triggered,
		this, &MainWindow::saveShow );

	m_save_show_as_action = new QAction( "&Save Show As", this );
	m_save_show_as_action->setStatusTip( "Save show as" );
	connect(
		m_save_show_as_action, &QAction::triggered,
		this, &MainWindow::saveShowAs );

	m_open_show_action = new QAction( "&Open Show", this );
	m_open_show_action->setStatusTip( "Open show" );
	connect(
		m_open_show_action, &QAction::triggered,
		this, &MainWindow::openShow );

	m_exit_action = new QAction( "E&xit", this );
	m_exit_action->setShortcuts( QKeySequence::Quit );
	m_exit_action->setStatusTip( "Exit the application" );
	connect(
		m_exit_action, &QAction::triggered,
		this, &MainWindow::close );

	// edit

	m_edit_preferences_action = new QAction( "Preferences", this );
	connect(
		m_edit_preferences_action, &QAction::triggered,
		this, &MainWindow::editPreferences );

	// about

	m_about_action = new QAction( "&About", this );
	m_about_action->setStatusTip( "About noises" );
	connect(
		m_about_action, &QAction::triggered,
		this, &MainWindow::about );

	m_about_qt_action = new QAction( "About &Qt", this );
	m_about_qt_action->setStatusTip( "About Qt" );
	connect(
		m_about_qt_action, &QAction::triggered,
		qApp, &QApplication::aboutQt );

	// toolbar actions

	m_new_audio_cue_action = new QAction(
		QIcon( ":/images/audio_cue_32x32.png" ),
		"&New Audio Cue", this );
	m_new_audio_cue_action->setStatusTip( "Create a new audio cue" );
	connect(
		m_new_audio_cue_action, &QAction::triggered,
		this, &MainWindow::newAudioCue );

	m_new_control_cue_action = new QAction(
		QIcon( ":/images/control_cue_32x32.png" ),
		"&New Control Cue",
		this );
	m_new_control_cue_action->setStatusTip( "Create a new control cue" );
	connect(
		m_new_control_cue_action, &QAction::triggered,
		this, &MainWindow::newControlCue );

	m_new_wait_cue_action = new QAction(
		QIcon( ":/images/wait_cue_32x32.png" ),
		"&New Wait Cue",
		this );
	m_new_wait_cue_action->setStatusTip( "Create a new wait cue" );
	connect(
		m_new_wait_cue_action, &QAction::triggered,
		this, &MainWindow::newWaitCue );

	m_new_group_cue_action = new QAction(
		QIcon( ":/images/group_cue_32x32.png" ),
		"&New Group Cue", this );
	m_new_group_cue_action->setStatusTip( "Create a new group cue" );
	connect(
		m_new_group_cue_action, &QAction::triggered,
		this, &MainWindow::newGroupCue );

	// cue actions

	m_play_cue_action = new QAction( "Play Cue", m_cue_list );
	m_play_cue_action->setShortcut( QKeySequence( Qt::Key_Space ) );
	connect(
		m_play_cue_action, &QAction::triggered,
		this, &MainWindow::playCue );

	m_delete_cue_action = new QAction( "Delete Cue", m_cue_list );
	m_delete_cue_action->setShortcut( QKeySequence( Qt::Key_Delete ) );
	connect(
		m_delete_cue_action, &QAction::triggered,
		this, &MainWindow::deleteCue );

	m_stop_all_cues_action = new QAction( "Play Cue", m_cue_list );
	m_stop_all_cues_action->setShortcut( QKeySequence( Qt::Key_Escape ) );
	connect(
		m_stop_all_cues_action, &QAction::triggered,
		this, &MainWindow::stopAllCues );

	// setup cue list actions

	m_cue_list->addAction( m_play_cue_action );
	m_cue_list->addAction( m_delete_cue_action );
	m_cue_list->addAction( m_stop_all_cues_action );
}

void MainWindow::createMenus()
{
	m_file_menu = menuBar()->addMenu( "&File" );
	m_file_menu->addAction( m_new_show_action );
	m_file_menu->addAction( m_open_show_action );
	m_file_menu->addAction( m_save_show_action );
	m_file_menu->addAction( m_save_show_as_action );
	m_file_menu->addSeparator();
	m_file_menu->addAction( m_exit_action );

	m_edit_menu = menuBar()->addMenu( "&Edit" );
	m_edit_menu->addAction( m_edit_preferences_action );

	menuBar()->addSeparator();

	m_help_menu = menuBar()->addMenu( "&Help" );
	m_help_menu->addAction( m_about_action );
	m_help_menu->addAction( m_about_qt_action );
}

void MainWindow::createToolBars()
{
	m_tool_bar = addToolBar( "Tools" );
	m_tool_bar->addAction( m_new_audio_cue_action );
	m_tool_bar->addAction( m_new_control_cue_action );
	m_tool_bar->addAction( m_new_wait_cue_action );
	m_tool_bar->addAction( m_new_group_cue_action );
}

void MainWindow::createStatusBar()
{
	statusBar()->showMessage( "Ready" );
}

} /* namespace noises */

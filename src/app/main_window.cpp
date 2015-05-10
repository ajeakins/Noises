
#include <assert.h>

#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QSettings>
#include <QToolBar>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <cue/audio_cue_model_item.h>
#include <cue/control_cue_model_item.h>
#include <cue/cue_model_item.h>
#include <cue/group_cue_model_item.h>
#include <cue/wait_cue_model_item.h>

#include <cue_dialogs/api.h>

#include <cue_widget/cue_widget.h>

#include "application.h"
#include "main_window.h"
#include "preferences.h"

#define WITH_GROUP_CUES 0

namespace noises
{

MainWindow::MainWindow()
{
	createWidgets();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	loadSettings();

	updateWindowTitle();

	resize( 600, 500 );
}

void MainWindow::setSavePending()
{
	m_save_pending = true;
	updateWindowTitle();
}

void MainWindow::closeEvent( QCloseEvent* event )
{
	if ( m_save_pending )
	{
		QMessageBox message(
			QMessageBox::Question,
			"Unsaved Changes",
			"There are unsaved changes, are you sure you want to quit?",
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
			this );
		int ret = message.exec();
		if ( ret == QMessageBox::Save )
		{
			saveShow();
		}
		else if ( ret == QMessageBox::Cancel )
		{
			return;
		}
	}

	// check state and confirm
	saveSettings();

	event->accept();
}

void MainWindow::saveSettings()
{
	QSettings settings( "Noises", "Noises" );
	settings.setValue( "filename", m_current_file_name );
}

void MainWindow::loadSettings()
{
	QSettings settings( "Noises", "Noises" );
	QString file_name = settings.value( "filename", "" ).toString();
	if ( !file_name.isEmpty() )
	{
		openShow( file_name );
	}
}

void MainWindow::updateWindowTitle()
{
	QString title = "Noises";
	if ( !m_current_file_name.isEmpty() )
	{
		title += " - ";
		title += m_current_file_name;

		if ( m_save_pending )
		{
			title += "*";
		}
	}

	setWindowTitle( title );
}

void MainWindow::newShow()
{}

void MainWindow::saveShow()
{
	if ( m_current_file_name.isEmpty() )
	{
		saveShowAs();
	}

	QJsonArray cues;
	m_cue_list->writeSettings( cues );

	QJsonObject root;
	root["cues"] = cues;

	QFile file( m_current_file_name ); // scoped
	file.open( QIODevice::WriteOnly );

	QJsonDocument document( root );
	file.write( document.toJson() );

	m_save_pending = false;
	updateWindowTitle();
}

void MainWindow::saveShowAs()
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

	m_current_file_name = filename;
	updateWindowTitle();

	saveShow();
}

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

	openShow( filename );
}

void MainWindow::openShow( const QString& file_name )
{
	QFile file( file_name ); // scoped
	file.open( QIODevice::ReadOnly );

	if ( !file.isOpen() )
	{
		QMessageBox::critical( this, "Error Loading File", "Unable to load show file." );
		return;
	}

	m_current_file_name = file_name;
	updateWindowTitle();

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
		m_cue_list->readSettings( cues.toArray() );
	}
}

void MainWindow::createCue( CueType type )
{
	CueModelItem* item = m_cue_list->createCue( type );
	showCueEditDialog( item, m_cue_list->getDataMapperForSelection(), true, this );
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
	CueModelItem* item = m_cue_list->getCurrentItem();
	if ( !item )
	{
		return;
	}
	item->execute();
}

void MainWindow::deleteCue()
{
	m_cue_list->deleteCurrentCue();
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
	m_cue_list = new CueWidget( this );
	connect(
		m_cue_list, &CueWidget::cueSettingChanged,
		this, &MainWindow::setSavePending );
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
		this, [this](){openShow();} );

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
		this, [this](){ createCue( CueType_Audio ); } );

	m_new_control_cue_action = new QAction(
		QIcon( ":/images/control_cue_32x32.png" ),
		"&New Control Cue",
		this );
	m_new_control_cue_action->setStatusTip( "Create a new control cue" );
	connect(
		m_new_control_cue_action, &QAction::triggered,
		this, [this](){ createCue( CueType_Control ); } );

	m_new_wait_cue_action = new QAction(
		QIcon( ":/images/wait_cue_32x32.png" ),
		"&New Wait Cue",
		this );
	m_new_wait_cue_action->setStatusTip( "Create a new wait cue" );
	connect(
		m_new_wait_cue_action, &QAction::triggered,
		this, [this](){ createCue( CueType_Wait ); } );

	m_new_group_cue_action = new QAction(
		QIcon( ":/images/group_cue_32x32.png" ),
		"&New Group Cue", this );
	m_new_group_cue_action->setStatusTip( "Create a new group cue" );
	connect(
		m_new_group_cue_action, &QAction::triggered,
		this, [this](){ createCue( CueType_Group ); } );

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
#if WITH_GROUP_CUES
	m_tool_bar->addAction( m_new_group_cue_action );
#endif
}

void MainWindow::createStatusBar()
{
	statusBar()->showMessage( "Ready" );
}

} /* namespace noises */

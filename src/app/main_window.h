#pragma once

#include <QMainWindow>

#include <cue/types.h>

class QAction;
class QMenu;
class QDataWidgetMapper;

namespace noises
{
	class CueWidget;
	class CueModelItem;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow();

		void setSavePending();

	protected:
		void closeEvent( QCloseEvent* event );

	private Q_SLOTS:
		// Menu Slots
		void newShow();
		void saveShow();
		void saveShowAs();
		void openShow();
		void openShow( const QString& file_name );

		void editPreferences();

		void about();

		// Cue Slots
		void playCue();
		void deleteCue();
		void stopAllCues();

	private:
		void createWidgets();
		void createActions();
		void createMenus();
		void createToolBars();
		void createStatusBar();

		void saveSettings();
		void loadSettings();

		void updateWindowTitle();

		void createCue( CueType type );

	private:
		QString m_current_file_name;
		bool m_save_pending = false;

		// Widgets
		CueWidget* m_cue_list;

		// Menus
		QMenu* m_file_menu;
		QMenu* m_help_menu;
		QMenu* m_edit_menu;

		// Toolbar
		QToolBar* m_tool_bar;

		// Toolbar Actions
		QAction* m_new_audio_cue_action;
		QAction* m_new_control_cue_action;
		QAction* m_new_wait_cue_action;
		QAction* m_new_group_cue_action;

		// Menu Actions
		QAction* m_new_show_action;
		QAction* m_save_show_action;
		QAction* m_save_show_as_action;
		QAction* m_open_show_action;
		QAction* m_exit_action;

		QAction* m_edit_preferences_action;

		QAction* m_about_action;
		QAction* m_about_qt_action;

		// Cue Actions
		QAction* m_play_cue_action;
		QAction* m_delete_cue_action;
		QAction* m_stop_all_cues_action;
	};

} /* namespace noises */

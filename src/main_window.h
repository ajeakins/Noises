 #pragma once

#include <QMainWindow>
#include <QTreeView>

#include <audio/hub.h>

#include "types.h"

class QAction;
class QMenu;
class QTreeView;
class QDataWidgetMapper;

namespace noises
{
	class CueModel;
	class CueModelItem;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow();

	protected:
		void closeEvent( QCloseEvent* event );

	private Q_SLOTS:
		// Menu Slots
		void newShow();

		void editPreferences();

		void about();

		// Toolbar Slots
		void newAudioCue();
		void newControlCue();
		void newWaitCue();

		// Cue Slots
		void playCue();
		void editCue( QModelIndex index );
		void stopAllCues();

	private:
		void createActions();
		void createMenus();
		void createToolBars();
		void createStatusBar();

		QDataWidgetMapper* getDataMapper();

		CueModelItem* createCue( CueType type );

	private:
		// Widgets
		QTreeView* m_cue_list;
		CueModel* m_cue_model;
		QString m_current_show_file;

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

		// Menu Actions
		QAction* m_new_show_action;
		QAction* m_exit_action;

		QAction* m_edit_preferences_action;

		QAction* m_about_action;
		QAction* m_about_qt_action;

		// Cue Actions
		QAction* m_play_cue_action;
		QAction* m_stop_all_cues_action;
	};

} /* namespace noises */
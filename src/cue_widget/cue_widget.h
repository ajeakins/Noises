#pragma once

#include <QTreeView>

class QDataWidgetMapper;

namespace noises
{
	class CueModel;

	class CueWidget : public QTreeView
	{
		Q_OBJECT

	public:
		CueWidget( QWidget* parent = nullptr );

		// cue management

		CueModelItem* createCue( CueType type );

		// selection

		CueModelItem* getCurrentItem();

		void selectNextCue();

		QDataWidgetMapper* getDataMapperForSelection();

		// settings

		void readSettings( const QJsonArray& settings );

		void writeSettings( QJsonArray& settings ) const;

	Q_SIGNALS:
		// report a setting change so the main window can set save
		// state pending
		void cueSettingChanged();

	public Q_SLOTS:
		void deleteCurrentCue();

	private Q_SLOTS:
		void editCue( QModelIndex index );

		void editCurrentCue();

		// increment selection when we get the signal a cue
		// is done playing
		void cueDone( CueModelItem* item );

	private:
		void contextMenuEvent( QContextMenuEvent* event );

	private:
		CueModel* m_cue_model;

		QAction* m_edit_cue_action;
		QAction* m_delete_cue_action;
	};

} /* namespace noises */

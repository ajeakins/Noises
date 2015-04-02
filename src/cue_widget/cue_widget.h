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

		void deleteCurrentCue();

		// selection

		CueModelItem* getCurrentItem();

		void selectNextCue();

		QDataWidgetMapper* getDataMapperForSelection();

		// settings

		void readSettings( const QJsonArray& settings );

		void writeSettings( QJsonArray& settings ) const;

	private Q_SLOTS:
		void editCue( QModelIndex index );

	private:
		CueModel* m_cue_model;
	};

} /* namespace noises */

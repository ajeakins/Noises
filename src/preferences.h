#pragma once

#include <QDialog>
#include <QMap>
#include <QVariant>

namespace noises
{
	class Preferences
	{
	public:
		Preferences();

		void getKeys( QList< QString >& keys ) const;

		QVariant getValue( const QString& key ) const;
		void setValue(
			const QString& key,
			const QVariant& value );

		QVariant::Type getValueType( const QString& key ) const;

	private:
		QMap< QString, QVariant > m_data;
	};

	class PreferencesDialog: public QDialog
	{
		Q_OBJECT;

	public:
		PreferencesDialog(
			Preferences& preferences,
			QWidget* parent = 0 );

	public Q_SLOTS:
		virtual void accept();

	private:
		void createWidgets();

		void writeSettings();

	private:
		Preferences& m_preferences;

		typedef QMap< QString, QWidget* > WidgetMap;

		WidgetMap m_widgets;
	};

} /* namespace noises */
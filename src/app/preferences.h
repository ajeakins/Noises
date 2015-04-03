#pragma once

#include <QDialog>
#include <QMap>
#include <QVariant>

namespace noises
{
	class Preferences
	{
		friend class PreferencesDialog;

	public:
		Preferences();

	public:
		bool getDefaultStereoLink() const;

		unsigned int getOutputCount() const;

		unsigned int getSampleRate() const;

	private:
		void getKeys( QList< QString >& keys ) const;

		QVariant getValue( const QString& key ) const;
		void setValue(
			const QString& key,
			const QVariant& value );

		QVariant::Type getValueType( const QString& key ) const;

	private:
		struct Preference
		{
			Preference()
			{}

			Preference(
				QVariant _value,
				QString _tooltip )
			:
				value( _value ),
				tooltip( _tooltip )
			{}

			QVariant value;
			QString tooltip;
		};

		QMap< QString, Preference > m_data;
	};

	class PreferencesDialog: public QDialog
	{
		Q_OBJECT
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

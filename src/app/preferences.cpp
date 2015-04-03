
#include <assert.h>

#include <QtGui>

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLayout>
#include <QLabel>
#include <QSpinBox>

#include "preferences.h"

namespace noises
{

// Preferences

Preferences::Preferences()
{
	m_data["output count"] = Preference( (uint)2, "Some tooltip" );
	m_data["stereo link"] = Preference( true, "Some tooltip" );
}

bool Preferences::getDefaultStereoLink() const
{
	return m_data["stereo link"].value.toBool();
}

unsigned int Preferences::getOutputCount() const
{
	return m_data["output count"].value.toUInt();
}

unsigned int Preferences::getSampleRate() const
{
	return 44100;
}

void Preferences::getKeys( QList< QString >& keys ) const
{
	keys = m_data.keys();
}

QVariant Preferences::getValue( const QString& key ) const
{
	return m_data[key].value;
}

void Preferences::setValue(
	const QString& key,
	const QVariant& value )
{
	m_data[key].value = value;
}

QVariant::Type Preferences::getValueType( const QString& key ) const
{
	return m_data[key].value.type();
}

// Preferences Dialog

PreferencesDialog::PreferencesDialog(
	Preferences& preferences,
	QWidget* parent )
:
	QDialog( parent ),
	m_preferences( preferences )
{
	setWindowTitle( "Preferences" );

	createWidgets();
}

void PreferencesDialog::accept()
{
	writeSettings();
	QDialog::accept();
}

void PreferencesDialog::createWidgets()
{
	QGridLayout* prefs_layout = new QGridLayout;

	QList< QString > keys;
	m_preferences.getKeys( keys );

	for( int i = 0; i != keys.count(); ++i )
	{
		QLabel* label = new QLabel( keys[i], this );

		prefs_layout->addWidget( label, i, 0 );

		QVariant::Type type = m_preferences.getValueType( keys[i] );

		switch( type )
		{
			case QVariant::Bool:
			{
				QCheckBox* checkbox = new QCheckBox( this );
				bool value = m_preferences.getValue( keys[i] ).toBool();
				checkbox->setChecked( value );

				m_widgets[keys[i]] = checkbox;

				prefs_layout->addWidget( checkbox, i, 1 );
				break;
			}
			case QVariant::UInt:
			{
				QSpinBox* spinbox = new QSpinBox( this );
				int value = m_preferences.getValue( keys[i] ).toInt();
				spinbox->setValue( value );

				m_widgets[keys[i]] = spinbox;

				prefs_layout->addWidget( spinbox, i, 1 );
				break;
			}
			default:
			{
				QString warning( "Uknown preference type " );
				warning += "[";
				warning += QVariant::typeToName( type );
				warning += "]";
				QLabel* warning_label = new QLabel( warning );

				prefs_layout->addWidget( warning_label, i, 1 );

				break;
			}
		}
	}

	// buttons

	QDialogButtonBox* buttons = new QDialogButtonBox(
		QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

	connect(
		buttons, SIGNAL( accepted() ),
		this, SLOT( accept() ) );

	connect(
		buttons, SIGNAL( rejected() ),
		this, SLOT( reject() ) );

	// layout

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addLayout( prefs_layout );
	layout->addWidget( buttons );

	setLayout( layout );
}

void PreferencesDialog::writeSettings()
{
	for (
		WidgetMap::const_iterator itr = m_widgets.begin();
		itr != m_widgets.end();
		++itr )
	{
		const QString& key = itr.key();
		QWidget* widget = itr.value();

		switch( m_preferences.getValueType( key ) )
		{
			case QVariant::Bool:
			{
				QCheckBox* checkbox = dynamic_cast< QCheckBox* >( widget );
				assert( checkbox );

				bool value = checkbox->isChecked();
				m_preferences.setValue( key, value );
				break;
			}
			case QVariant::UInt:
			{
				QSpinBox* spinbox = dynamic_cast< QSpinBox* >( widget );
				assert( spinbox );

				uint value = spinbox->value();
				m_preferences.setValue( key, value );
				break;
			}
			default:
				qWarning() << "WARNING: not writting setting\n";
				break;
		}
	}
}

} /* namespace noises */

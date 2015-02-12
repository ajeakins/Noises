
#include <QApplication>
#include <QDataWidgetMapper>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFileInfo>
#include <QTimeEdit>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QStyle>

#include <audio/manager.h>

#include "audio_cue_dialog.h"

namespace noises
{

static const QString secondTimeFormat = "ss.zzz 's'";
static const QString minuteTimeFormat = "mm'm' ss.zzz 's'";
static const QString hourTimeFormat = "hh'h' mm'm' ss.zzz";

AudioCueDialog::AudioCueDialog(
	AudioCueModelItem* cue,
	QDataWidgetMapper* mapper,
	QWidget* parent )
:
	CueDialog( mapper, parent ),
	m_cue( cue )
{
	m_player = Application::getAudioManager().createPlayer( this );

	setWindowTitle( "Edit Audio Cue" );
	setWindowIcon( QIcon( ":images/audio_cue.png" ) );

	createCueWidgets();

	// read the settings and write into widgets
	readSettings();

	setSizePolicy(
		QSizePolicy(
			QSizePolicy::Minimum,
			QSizePolicy::Minimum ) );
	resize( 0, 0 );
}

AudioCueDialog::~AudioCueDialog()
{
	m_player->stop();
}

void AudioCueDialog::accept()
{
	writeSettings();
	CueDialog::accept();
}

void AudioCueDialog::onFilenameChanged()
{
	QString filepath = m_file_edit->text();
	QFileInfo file_info( filepath );

	getEditor( 1 )->setText( file_info.baseName() );

	m_player->setFilename( filepath );
	resetTimes();
}

void AudioCueDialog::resetTimes()
{
	QTime time( 0, 0, 0, 0 );
	m_player->getDuration( time );
	m_end_time->setTime( time );

	if ( time.hour() > 0 )
	{
		setTimeDisplayFormat( hourTimeFormat );
	}
	else if ( time.minute() > 0 )
	{
		setTimeDisplayFormat( minuteTimeFormat );
	}
	else
	{
		setTimeDisplayFormat( secondTimeFormat );
	}
}

void AudioCueDialog::volumeChanged()
{
	widgets::MatrixSettings settings;
	m_matrix->writeSettings( settings );
	m_player->setVolume( settings );
}

void AudioCueDialog::writeSettings() const
{
	AudioCueSettings& settings = m_cue->getSettings();

	settings.file_name = m_file_edit->text();

	settings.start_time = m_start_time->time();
	settings.end_time = m_end_time->time();

	settings.start_fade = m_fade_in_time->time();
	settings.end_fade = m_fade_out_time->time();

	m_matrix->writeSettings( settings.levels );
}

void AudioCueDialog::readSettings()
{
	const AudioCueSettings& settings = m_cue->getSettings();

	m_file_edit->setText( settings.file_name );

	m_start_time->setTime( settings.start_time );
	m_end_time->setTime( settings.end_time );

	m_fade_in_time->setTime( settings.start_fade );
	m_fade_out_time->setTime( settings.end_fade );

	m_matrix->readSettings( settings.levels );
	// update output count
	m_matrix->setOutputs( Application::getPreferences().getOutputCount() );
}

void AudioCueDialog::createCueWidgets()
{
	createWidgets();

	// file

	m_file_edit = new widgets::FileLineEdit( this );

	connect(
		m_file_edit, SIGNAL( filenameChanged() ),
		this, SLOT( onFilenameChanged() ) );

	QVBoxLayout* file_layout = new QVBoxLayout();
	file_layout->setContentsMargins( 0, 0, 0, 0 );
	file_layout->addWidget( m_file_edit );

	QGroupBox* file_group_box = new QGroupBox( "File", this );
	file_group_box->setLayout( file_layout );

	// preview

	QLabel* elapsed_label =  new QLabel( "Remaining:", this );
	m_elapsed_time = new QTimeEdit( this );
	m_elapsed_time->setReadOnly( true );

	QLabel* remaining_label = new QLabel( "Elapsed:", this );
	m_remaining_time = new QTimeEdit( this );
	m_remaining_time->setReadOnly( true );

	m_play_button = new QPushButton( this );
	m_play_button->setIcon( QIcon( ":images/play_32x32.png" ) );

	m_stop_button = new QPushButton( this );
	m_stop_button->setIcon( QIcon( ":images/stop_32x32.png" ) );

	connect(
		m_play_button, &QPushButton::released,
		[this](){ m_player->start(); } );

	connect(
		m_stop_button, &QPushButton::released,
		[this](){ m_player->stop(); } );

	QGridLayout* preview_times_grid = new QGridLayout();
	preview_times_grid->setContentsMargins( 0, 0, 0, 0 );

	preview_times_grid->addWidget( remaining_label, 0, 0 );
	preview_times_grid->addWidget( m_remaining_time, 0, 1 );
	preview_times_grid->addWidget( elapsed_label, 0, 2 );
	preview_times_grid->addWidget( m_elapsed_time, 0, 3 );

	QHBoxLayout* preview_buttons = new QHBoxLayout;
	preview_buttons->setContentsMargins( 0, 0, 0, 0 );
	preview_buttons->addWidget( m_play_button );
	preview_buttons->addWidget( m_stop_button );
	preview_buttons->addStretch();

	QVBoxLayout* preview_layout = new QVBoxLayout;
	preview_layout->addLayout( preview_times_grid );
	preview_layout->addLayout( preview_buttons );

	QGroupBox* preview_group_box = new QGroupBox( "Preview", this );
	preview_group_box->setLayout( preview_layout );

	// times

	QLabel* start_label =  new QLabel( "Start:", this );
	m_start_time = new QTimeEdit( this );

	QLabel* end_label = new QLabel( "End:", this );
	m_end_time = new QTimeEdit( this );

	QLabel* fade_in_label = new QLabel( "Fade in:", this );
	m_fade_in_time = new QTimeEdit( this );

	QLabel* fade_out_label = new QLabel( "Fade out:", this );
	m_fade_out_time = new QTimeEdit( this );

	setTimeDisplayFormat( secondTimeFormat );

	m_reset_times_button = new QPushButton( this );
	m_reset_times_button->setIcon( QIcon( ":images/reload_32x32.png") );
	m_reset_times_button->setToolTip( "Reset start and end to those in the file" );

	connect(
		m_reset_times_button, SIGNAL( released() ),
		this, SLOT( resetTimes() ) );

	QGridLayout* times_grid = new QGridLayout();
	times_grid->setContentsMargins( 0, 0, 0, 0 );

	times_grid->addWidget( start_label, 0, 0 );
	times_grid->addWidget( m_start_time, 0, 1 );
	times_grid->addWidget( end_label, 0, 2 );
	times_grid->addWidget( m_end_time, 0, 3 );

	times_grid->addWidget( m_reset_times_button, 0, 4 );

	times_grid->addWidget( fade_in_label, 1, 0 );
	times_grid->addWidget( m_fade_in_time, 1, 1 );
	times_grid->addWidget( fade_out_label, 1, 2 );
	times_grid->addWidget( m_fade_out_time, 1, 3 );

	QHBoxLayout* times_layout = new QHBoxLayout;
	times_layout->setContentsMargins( 0, 0, 0, 0 );
	times_layout->addLayout( times_grid );
	times_layout->addStretch();

	QGroupBox* times_group_box = new QGroupBox( "Times", this );
	times_group_box->setLayout( times_layout );

	// audio matrix

	m_matrix = new widgets::Matrix( 2, 2, this );

	connect(
		m_matrix, SIGNAL( volumeChanged() ),
		this, SLOT( volumeChanged() ) );

	QHBoxLayout* levels_layout = new QHBoxLayout();
	levels_layout->addWidget( m_matrix );
	levels_layout->addStretch();
	levels_layout->setContentsMargins( 0, 0, 0, 0 );

	QGroupBox* levels_group_box = new QGroupBox( "Levels", this );
	levels_group_box->setLayout( levels_layout );

	m_layout->addWidget( file_group_box );
	m_layout->addWidget( preview_group_box );
	m_layout->addWidget( times_group_box );
	m_layout->addWidget( levels_group_box );
}

void AudioCueDialog::setTimeDisplayFormat( QString format )
{
	m_start_time->setDisplayFormat( format );
	m_end_time->setDisplayFormat( format );
	m_fade_in_time->setDisplayFormat( format );
	m_fade_out_time->setDisplayFormat( format );
}

} /* namespace noises */
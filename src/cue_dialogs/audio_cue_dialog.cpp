
#include <QApplication>
#include <QDataWidgetMapper>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QStyle>

#include <audio/manager.h>

#include "audio_cue_dialog.h"

namespace noises
{

AudioCueDialog::AudioCueDialog(
	AudioCueModelItem* cue,
	QDataWidgetMapper* mapper,
	QWidget* parent )
:
	CueDialog( mapper, parent ),
	m_cue( cue ),
	m_player( audio::Player::create() )
{
	Application::getAudioManager().registerPlayer( m_player );

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
	Application::getAudioManager().unregisterPlayer( m_player );
}

void AudioCueDialog::accept()
{
	writeSettings();
	CueDialog::accept();
}

void AudioCueDialog::play()
{
	m_player->start();
}

void AudioCueDialog::stop()
{
	m_player->stop();
}

void AudioCueDialog::onFilenameChanged()
{
	std::cout << "onFilenameChanged" << std::endl;
	m_player->setFilename( m_file_edit->text() );
}

void AudioCueDialog::writeSettings()
{
	AudioCueSettings& settings = m_cue->getSettings();

	settings.file_name = m_file_edit->text();

	settings.start_time = m_start_time->value();
	settings.end_time = m_end_time->value();

	settings.start_fade = m_fade_in_time->value();
	settings.end_fade = m_fade_out_time->value();

	m_matrix->writeSettings( settings.levels );
}

void AudioCueDialog::readSettings()
{
	const AudioCueSettings& settings = m_cue->getSettings();

	m_file_edit->setText( settings.file_name );

	m_start_time->setValue( settings.start_time );
	m_end_time->setValue( settings.end_time );

	m_fade_in_time->setValue( settings.start_fade );
	m_fade_out_time->setValue( settings.end_fade );

	m_matrix->readSettings( settings.levels );
}

void AudioCueDialog::createCueWidgets()
{
	createWidgets();

	// file

	QIcon icon;

	m_file_edit = new widgets::FileLineEdit( this );

	connect(
		m_file_edit, SIGNAL( filenameChanged() ),
		this, SLOT( onFilenameChanged() ) );

	m_play_button = new QPushButton( this );
	icon = QApplication::style()->standardIcon( QStyle::SP_MediaPlay );
	m_play_button->setIcon( icon );

	m_stop_button = new QPushButton( this );
	icon = QApplication::style()->standardIcon( QStyle::SP_MediaStop );
	m_stop_button->setIcon( icon );

	connect(
		m_play_button, SIGNAL( released() ),
		this, SLOT( play() ) );

	connect(
		m_stop_button, SIGNAL( released() ),
		this, SLOT( stop() ) );

	m_reset_times_button = new QPushButton( this );
	icon = QApplication::style()->standardIcon( QStyle::SP_BrowserReload );
	m_reset_times_button->setIcon( icon );
	m_reset_times_button->setToolTip( "Reset start and end to those in the file" );

	QHBoxLayout* file_options = new QHBoxLayout;
	file_options->setContentsMargins( 0, 0, 0, 0 );
	file_options->addWidget( m_play_button );
	file_options->addWidget( m_stop_button );
	file_options->addStretch();
	file_options->addWidget( m_reset_times_button );

	QVBoxLayout* file_layout = new QVBoxLayout();
	file_layout->setContentsMargins( 0, 0, 0, 0 );
	file_layout->addWidget( m_file_edit );
	file_layout->addItem( file_options );

	QGroupBox* file_group_box = new QGroupBox( "File", this );
	file_group_box->setLayout( file_layout );

	// times

	QLabel* start_label =  new QLabel( "Start:", this );
	m_start_time = new QDoubleSpinBox( this );
	m_start_time->setSuffix( "s" );

	QLabel* end_label = new QLabel( "End:", this );
	m_end_time = new QDoubleSpinBox( this );
	m_end_time->setSuffix( "s" );

	QLabel* fade_in_label = new QLabel( "Fade in:", this );
	m_fade_in_time = new QDoubleSpinBox( this );
	m_fade_in_time->setSuffix( "s" );

	QLabel* fade_out_label = new QLabel( "Fade out:", this );
	m_fade_out_time = new QDoubleSpinBox( this );
	m_fade_out_time->setSuffix( "s" );

	QGridLayout* times_grid = new QGridLayout();
	times_grid->setContentsMargins( 0, 0, 0, 0 );

	times_grid->addWidget( start_label, 0, 0 );
	times_grid->addWidget( m_start_time, 0, 1 );
	times_grid->addWidget( end_label, 0, 2 );
	times_grid->addWidget( m_end_time, 0, 3 );

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

	QHBoxLayout* levels_layout = new QHBoxLayout();
	levels_layout->addWidget( m_matrix );
	levels_layout->addStretch();
	levels_layout->setContentsMargins( 0, 0, 0, 0 );

	QGroupBox* levels_group_box = new QGroupBox( "Levels", this );
	levels_group_box->setLayout( levels_layout );

	m_layout->addWidget( file_group_box );
	m_layout->addWidget( times_group_box );
	m_layout->addWidget( levels_group_box );
}

} /* namespace noises */
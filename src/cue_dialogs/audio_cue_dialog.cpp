
#include <QDataWidgetMapper>
#include <QFileInfo>
#include <QTimeEdit>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QTabWidget>

#include <audio/manager.h>
#include <audio/volume_matrix.h>
#include <audio/player/types.h>

#include <cue/audio_cue_model_item.h>

#include <utils/time.h>

#include <widgets/time_label.h>
#include <widgets/file_line_edit.h>
#include <widgets/matrix.h>
#include <widgets/pan.h>
#include <widgets/time_label.h>

#include "audio_cue_dialog.h"

#define WITH_PAN 0

namespace noises
{

AudioCueDialog::AudioCueDialog(
	AudioCueModelItem* cue,
	QDataWidgetMapper* mapper,
	bool create,
	QWidget* parent )
:
	CueDialog( mapper, create, parent ),
	m_cue( cue )
{
	m_player = cue->getPlayer();

	connect(
		m_player.data(), &audio::Player::timeUpdated,
		this, &AudioCueDialog::playerTimeChanged );

	setDialogTitle( cue->getType() );
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

void AudioCueDialog::onFilenameChanged()
{
	QString filepath = m_file_edit->text();
	QFileInfo file_info( filepath );

	QString cue_name = file_info.baseName();
	cue_name.replace( "_", " " );

	getEditor( 1 )->setText( cue_name );

	m_player->setFilename( filepath );
	m_duration = m_player->getDuration();

	setTimeDisplayFormat(utils::timeFormat(m_duration));

	resetTimes();
	playerTimeChanged( QTime( 0, 0, 0 ) );
}

void AudioCueDialog::resetTimes()
{
	m_end_time->setTime( m_duration );
	setTimeDisplayFormat( utils::timeFormat( m_duration ) );
}

void AudioCueDialog::volumeChanged()
{
	audio::VolumeMatrix settings;
	m_matrix->writeSettings( settings );
	m_player->setVolume( settings );
}

void AudioCueDialog::playerTimeChanged( const QTime& time )
{
	m_elapsed_time->setTime( time );
	m_remaining_time->setTime( utils::subtract( m_duration, time ) );

	if ( utils::isZero( time ) )
	{
		m_play_pause_button->setIcon( QIcon( ":images/play_32x32.png" ) );
	}
}

void AudioCueDialog::playPause()
{
	if ( m_player->isPlaying() )
	{
		m_player->pause();
		m_play_pause_button->setIcon( QIcon( ":images/play_32x32.png" ) );
	}
	else
	{
		m_player->start();
		m_play_pause_button->setIcon( QIcon( ":images/pause_32x32.png" ) );
	}
}

void AudioCueDialog::stop()
{
	m_player->stop();
	m_play_pause_button->setIcon( QIcon( ":images/play_32x32.png" ) );
}

bool AudioCueDialog::writeSettings() const
{
	bool something_changed = false;

	AudioCueSettings& settings = m_cue->getSettings();

	NOISES_SET_VALUE( settings.file_name, m_file_edit->text(), something_changed )

	NOISES_SET_VALUE( settings.start_time, m_start_time->time(), something_changed )
	NOISES_SET_VALUE( settings.end_time, m_end_time->time(), something_changed )

	NOISES_SET_VALUE( settings.start_fade, m_fade_in_time->time(), something_changed )
	NOISES_SET_VALUE( settings.end_fade, m_fade_out_time->time(), something_changed )

	something_changed |= m_matrix->writeSettings( settings.levels );

	m_cue->updatePlayer();

	return something_changed;
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
	m_matrix->setOutputs( Application::getPreferences().getOutputCount() );

	m_player->setFilename( settings.file_name );

	volumeChanged();
	m_duration = m_player->getDuration();

	setTimeDisplayFormat(utils::timeFormat(m_duration));

	QTime time(0, 0, 0);
	playerTimeChanged(time);
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
	m_elapsed_time = new widgets::TimeLabel( this );
	m_elapsed_time->setPrefix( "Elapsed: " );

	m_remaining_time = new widgets::TimeLabel( this );
	m_remaining_time->setPrefix( "Remaining: " );

	m_play_pause_button = new QPushButton( this );
	m_play_pause_button->setIcon( QIcon( ":images/play_32x32.png" ) );

	m_stop_button = new QPushButton( this );
	m_stop_button->setIcon( QIcon( ":images/stop_32x32.png" ) );

	connect(
		m_play_pause_button, &QPushButton::released,
		this, &AudioCueDialog::playPause );

	connect(
		m_stop_button, &QPushButton::released,
		this, &AudioCueDialog::stop );

	QHBoxLayout* preview_layout = new QHBoxLayout;
	preview_layout->setContentsMargins( 0, 0, 0, 0 );

	preview_layout->addWidget( m_play_pause_button );
	preview_layout->addWidget( m_stop_button );

	preview_layout->addWidget( m_remaining_time );
	preview_layout->addWidget( m_elapsed_time );
	preview_layout->addStretch();

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

	setTimeDisplayFormat( utils::defaultTimeFormat() );

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

	QGroupBox* times_group_box = new QGroupBox( "Times", this );
	times_group_box->setLayout( times_grid );

#if WITH_PAN
	// pan
	widgets::Pan* pan = new widgets::Pan( this );

	connect(
		pan, &widgets::Pan::valueChanged,
		m_player.data(), &audio::AudioPlayer::setPan );

	QVBoxLayout* pan_layout = new QVBoxLayout;
	pan_layout->setContentsMargins( 0, 0, 0, 0 );
	pan_layout->addWidget( pan );

	QGroupBox* pan_group_box = new QGroupBox( "Pan", this );
	pan_group_box->setLayout( pan_layout );
#endif

	// detailed setings tabbed
	QTabWidget* tabs = new QTabWidget( this );

	// audio matrix
	m_matrix = new widgets::Matrix( 2, 2, this );

	connect(
		m_matrix, SIGNAL( volumeChanged() ),
		this, SLOT( volumeChanged() ) );

	QHBoxLayout* levels_layout = new QHBoxLayout();
	levels_layout->addWidget( m_matrix );
	levels_layout->addStretch();

	QWidget* levels_widget = new QWidget( this );
	levels_widget->setLayout( levels_layout );

	tabs->addTab( levels_widget, "Levels" );

	m_layout->addWidget( file_group_box );
	m_layout->addWidget( preview_group_box );
	m_layout->addWidget( times_group_box );
#if WITH_PAN
	m_layout->addWidget( pan_group_box );
#endif
	m_layout->addWidget( tabs );
}

void AudioCueDialog::setTimeDisplayFormat( QString format )
{
	m_elapsed_time->setDisplayFormat( format );
	m_remaining_time->setDisplayFormat( format );

	m_start_time->setDisplayFormat( format );
	m_end_time->setDisplayFormat( format );
	m_fade_in_time->setDisplayFormat( format );
	m_fade_out_time->setDisplayFormat( format );
}

} /* namespace noises */
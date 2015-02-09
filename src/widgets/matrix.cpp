
#include <iostream>

#include <QLabel>

#include "volume_dial.h"

#include "matrix.h"

namespace noises
{
namespace widgets
{

// Matrix Settings

MatrixSettings::MatrixSettings(
	unsigned int inputs,
	unsigned int outputs )
:
	m_inputs( inputs ),
	m_outputs( outputs ),
	m_minimum_value( VolumeDial::getMinimumVolume() ),
	m_stereo_link( false )
{
	m_volumes.resize( inputs );
	initialiseVolumes();
}

unsigned int MatrixSettings::getInputs() const
{
	return m_inputs;
}

void MatrixSettings::setInputs( unsigned int inputs )
{
	if ( inputs == m_inputs )
	{
		m_inputs = inputs;
		m_volumes.resize( inputs );
		initialiseVolumes();
	}
}

unsigned int MatrixSettings::getOutputs() const
{
	return m_outputs;
}

void MatrixSettings::setOutputs( unsigned int outputs )
{
	if ( outputs != m_outputs )
	{
		m_outputs = outputs;
		initialiseVolumes();
	}
}

float MatrixSettings::getVolume(
	unsigned int input,
	unsigned int output ) const
{
	if( input >= m_inputs || output >= m_outputs )
	{
		return m_minimum_value;
	}

	return m_volumes[input][output];
}

void MatrixSettings::setVolume(
	unsigned int input,
	unsigned int output,
	float volume )
{
	assert( input < m_inputs );
	assert( output < m_outputs );

	m_volumes[input][output] = volume;
}

bool MatrixSettings::getStereoLink() const
{
	return m_stereo_link;
}

void MatrixSettings::setStereoLink( bool linked )
{
	m_stereo_link = linked;
}


bool MatrixSettings::isMinimumVolume( float volume ) const
{
	return volume == m_minimum_value;
}

void MatrixSettings::initialiseVolumes()
{
	for ( unsigned int i = 0; i != m_inputs; ++i )
	{
		m_volumes[i].resize( m_outputs );

		for ( unsigned int j = 0; j != m_outputs; ++j )
		{
			m_volumes[i][j] = m_minimum_value;
		}
	}
}

// Matrix Widgets

Matrix::Matrix(
	unsigned int inputs,
	unsigned int outputs,
	QWidget* parent )
:
	QWidget( parent )
{
	setSizePolicy(
		QSizePolicy(
			QSizePolicy::Minimum,
			QSizePolicy::Minimum ) );
	createWidgets( inputs, outputs );
}

Matrix::~Matrix()
{}

void Matrix::setInputs( unsigned int inputs )
{
	if ( inputs != getInputs() )
	{
		updateVolumeGrid( inputs, getOutputs() );
	}
}

void Matrix::setOutputs( unsigned int outputs )
{
	if ( outputs != getOutputs() )
	{
		updateVolumeGrid( getInputs(), outputs );
	}
}

unsigned int Matrix::getInputs() const
{
	// subtract 1 to account for labels
	return m_volume_grid->columnCount() - 1;
}

unsigned int Matrix::getOutputs() const
{
	// subtract 1 to account for labels
	return m_volume_grid->rowCount() - 1;
}

VolumeDial* Matrix::getVolumeDial( unsigned int input, unsigned int output ) const
{
	// account for labels
	++input;
	++output;

	assert( ( int )input < m_volume_grid->rowCount() );
	assert( ( int )output < m_volume_grid->columnCount() );

	return ( VolumeDial* )( m_volume_grid->itemAtPosition( input, output )->widget() );
}

float Matrix::getVolume( unsigned int input, unsigned int output ) const
{
	VolumeDial* dial = getVolumeDial( input, output );
	return dial->getVolume();
}

void Matrix::setVolume( unsigned int input, unsigned int output, float value )
{
	VolumeDial* dial = getVolumeDial( input, output );
	dial->setVolume( value );
}

void Matrix::readSettings( const MatrixSettings& settings )
{
	if (
		settings.getInputs() != getInputs() ||
		settings.getOutputs() != getOutputs() )
	{
		updateVolumeGrid( settings.getInputs(), settings.getOutputs() );
	}

	for ( IndexDataMap::const_iterator itr = m_index_map.begin(); itr != m_index_map.end(); ++itr )
	{
		const IndexData& index_data = itr.value();
		VolumeDial* const& volume_dial = itr.key();

		volume_dial->setVolume( settings.getVolume( index_data.input, index_data.output) );
	}

	m_link->setChecked( settings.getStereoLink() );
}

void Matrix::writeSettings( MatrixSettings& settings ) const
{
	settings.setInputs( getInputs() );
	settings.setOutputs( getOutputs() );

	for ( IndexDataMap::const_iterator itr = m_index_map.begin(); itr != m_index_map.end(); ++itr )
	{
		const IndexData& index_data = itr.value();
		VolumeDial* const& volume_dial = itr.key();

		settings.setVolume( index_data.input, index_data.output, volume_dial->getVolume() );
	}

	settings.setStereoLink( m_link->isChecked() );
}

void Matrix::valueChanged( float value )
{
	if ( m_link->isChecked() )
	{

		VolumeDial* dial = ( VolumeDial* )sender();
		IndexDataMap::const_iterator index_data_itr = m_index_map.find( dial );
		if ( index_data_itr != m_index_map.end() )
		{
			unsigned int from_input, from_output, to_input, to_output;

			from_input = index_data_itr->input;
			from_output = index_data_itr->output;

			if ( ( from_input % 2 ) == 0 && ( from_output % 2 ) == 0 )
			{
				to_input = from_input + 1;
				to_output = from_output + 1;
			}
			else if ( ( from_input % 2 ) == 0 || ( from_output % 2 ) == 0 )
			{
				return;
			}
			else
			{
				to_input = from_input - 1;
				to_output = from_output - 1;
			}

			// account for labels
			++to_input;
			++to_output;

			QLayoutItem* item =  m_volume_grid->itemAtPosition( to_input, to_output );
			if ( !item )
			{
				return;
			}

			QWidget* widget = item->widget();
			VolumeDial* to_dial = dynamic_cast< VolumeDial* >( widget );
			if ( to_dial )
			{
				to_dial->setVolume( dial->getVolume() );
			}
		}
	}

	Q_EMIT volumeChanged();
}

void Matrix::createWidgets(
	unsigned int inputs,
	unsigned int outputs )
{
	m_volume_grid = new QGridLayout( this );
	m_volume_grid->setContentsMargins( 0, 0, 0, 0 );
	m_volume_grid->setSpacing( 0 );

	// stereo link widget
	m_link = new QCheckBox( "Link", this );
	m_link->setToolTip( "Link levels of stereo pairs" );

	m_volume_grid->addWidget( m_link, 0, 0 );

	updateVolumeGrid( inputs, outputs );
}

void Matrix::updateVolumeGrid(
	unsigned int inputs,
	unsigned int outputs )
{
	// TODO: optimise to only clear what isn't needed
	clearVolumeGrid();

		// set input labels
	for ( unsigned int i = 1; i != inputs + 1; ++i )
	{
		QString text;
		text.setNum( i );
		text.prepend( "In " );

		QLabel* label = new QLabel( text, this );
		m_volume_grid->addWidget( label, i, 0 );
	}

	// set output labels
	for ( unsigned int i = 1; i != outputs + 1; ++i )
	{
		QString text;
		text.setNum( i );
		text.prepend( "Out ");

		QLabel* label = new QLabel( text, this );
		m_volume_grid->addWidget( label, 0, i );
	}

	for ( unsigned int i = 1; i != inputs + 1; ++i )
	{
		for ( unsigned int j = 1; j != outputs + 1; ++j )
		{
			VolumeDial* dial = new VolumeDial( this );

			connect(
				dial, SIGNAL( valueChanged( float ) ),
				this, SLOT( valueChanged( float ) ) );

			m_volume_grid->addWidget( dial, i, j );

			IndexData index_data;
			index_data.input = i - 1;
			index_data.output = j - 1;
			m_index_map[dial] = index_data;
		}
	}
}

void Matrix::clearVolumeGrid()
{
	for ( int i = 0; i != m_volume_grid->rowCount(); ++i )
	{
		for ( int j = 0; j != m_volume_grid->columnCount(); ++j )
		{
			// skip the top left as it contains persistent widgets
			if ( i == 0 && j == 0 )
			{
				continue;
			}

			QLayoutItem* item = m_volume_grid->itemAtPosition( i, j );
			int index = m_volume_grid->indexOf( item->widget() );
			item = m_volume_grid->takeAt( index );
			delete item->widget();
			delete item;
		}
	}

	m_index_map.clear();
}

} /* namespace widgets */
} /* namespace noises */
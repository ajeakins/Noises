
#include <iostream>

#include <QLabel>

#include "volume_dial.h"

#include "matrix.h"

namespace noises
{
namespace widgets
{

// Matrix Settings

MatrixSettings::MatrixSettings()
{}

float MatrixSettings::volume(
	unsigned int input,
	unsigned int output )
{
	assert( input < m_inputs );
	assert( output < m_outputs );

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

bool MatrixSettings::isMinimumVolume( float volume )
{
	return volume == m_minimum_value;
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
	updateVolumeGrid( inputs, getOutputs() );

}

void Matrix::setOutputs( unsigned int outputs )
{
	updateVolumeGrid( outputs, getInputs() );
}

unsigned int Matrix::getInputs() const
{
	return m_volume_grid->columnCount();
}

unsigned int Matrix::getOutputs() const
{
	return m_volume_grid->rowCount();
}

float Matrix::getValue( unsigned int input, unsigned int output ) const
{
	assert( (int)input < m_volume_grid->rowCount() );
	assert( (int)output < m_volume_grid->columnCount() );

	VolumeDial* dial = ( VolumeDial* )( m_volume_grid->itemAtPosition( input, output )->widget() );
	return dial->getValue();
}

void Matrix::setValue( unsigned int input, unsigned int output, float value )
{
	assert( (int)input < m_volume_grid->rowCount() );
	assert( (int)output < m_volume_grid->columnCount() );

	VolumeDial* dial = ( VolumeDial* )( m_volume_grid->itemAtPosition( input, output )->widget() );
	dial->setValue( value );
}

void Matrix::readSettings( const MatrixSettings& settings )
{

}

void Matrix::writeSettings( MatrixSettings& settings ) const
{

}

void Matrix::valueChanged( float value )
{
	if ( !m_link->isChecked() )
	{
		return;
	}

	VolumeDial* dial = ( VolumeDial* )sender();
	IndexDataMap::const_iterator index_data_itr = m_index_map.find( dial );
	if ( index_data_itr != m_index_map.end() )
	{
		unsigned int to_input = index_data_itr->input + 1;
		unsigned int to_output = index_data_itr->output + 1;

		std::cout << to_input << ", " << to_output << std::endl;

		VolumeDial* to_dial = ( VolumeDial* )( m_volume_grid->itemAtPosition( to_input, to_output ) );
		if ( to_dial )
		{
			// TODO: broken :-( figure out how to block signals
			// dial->setValue( to_dial->getValue() );
		}
	}
}

void Matrix::createWidgets(
	unsigned int inputs,
	unsigned int outputs )
{
	m_volume_grid = new QGridLayout( this );
	m_volume_grid->setContentsMargins( 0, 0, 0, 0 );
	m_volume_grid->setSpacing( 0 );

	// stereo link widget
	m_link = new QCheckBox( "link", this );
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
	for ( int i = 1; i != m_volume_grid->rowCount(); ++i )
	{
		for ( int j = 1; j != m_volume_grid->columnCount(); ++j )
		{
			QLayoutItem* item = m_volume_grid->itemAtPosition( i, j );
			item->widget()->deleteLater();
		}
	}

	m_index_map.clear();
}

} /* namespace widgets */
} /* namespace noises */
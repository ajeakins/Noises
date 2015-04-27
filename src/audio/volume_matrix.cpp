
#include <assert.h>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "volume_matrix.h"

namespace noises
{
namespace audio
{

VolumeMatrix::VolumeMatrix(
	unsigned int inputs,
	unsigned int outputs )
:
	m_inputs( inputs ),
	m_outputs( outputs ),
	// m_minimum_value( VolumeDial::getMinimumVolume() ),
	m_minimum_value( -60 ), // TODO
	m_stereo_link( false )
{
	m_volumes.resize( inputs );
	initialiseVolumes();
}

unsigned int VolumeMatrix::getInputs() const
{
	return m_inputs;
}

void VolumeMatrix::setInputs( unsigned int inputs )
{
	if ( inputs == m_inputs )
	{
		m_inputs = inputs;
		m_volumes.resize( inputs );
		initialiseVolumes();
	}
}

unsigned int VolumeMatrix::getOutputs() const
{
	return m_outputs;
}

void VolumeMatrix::setOutputs( unsigned int outputs )
{
	if ( outputs != m_outputs )
	{
		m_outputs = outputs;
		initialiseVolumes();
	}
}

float VolumeMatrix::getVolume(
	unsigned int input,
	unsigned int output ) const
{
	if( input >= m_inputs || output >= m_outputs )
	{
		return m_minimum_value;
	}

	return m_volumes[input][output];
}

void VolumeMatrix::setVolume(
	unsigned int input,
	unsigned int output,
	float volume )
{
	assert( input < m_inputs );
	assert( output < m_outputs );

	m_volumes[input][output] = volume;
}

bool VolumeMatrix::getStereoLink() const
{
	return m_stereo_link;
}

void VolumeMatrix::setStereoLink( bool linked )
{
	m_stereo_link = linked;
}


bool VolumeMatrix::isMinimumVolume( float volume ) const
{
	return volume == m_minimum_value;
}

void VolumeMatrix::initialiseVolumes()
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

void VolumeMatrix::readSettings( const QJsonObject& settings )
{
	m_inputs = settings["inputs"].toInt();
	m_outputs = settings["outputs"].toInt();

	const QJsonArray volumes = settings["volumes"].toArray();

	m_volumes.resize( m_inputs );
	for ( unsigned int i = 0; i != m_inputs; ++i )
	{
		m_volumes[i].resize( m_outputs );
		const QJsonArray data = volumes[i].toArray();

		for ( unsigned int j = 0; j != m_outputs; ++j )
		{
			m_volumes[i][j] = data[j].toDouble();
		}
	}
}

void VolumeMatrix::writeSettings( QJsonObject& settings ) const
{
	settings.insert( "inputs", ( int )m_inputs );
	settings.insert( "outputs", ( int )m_outputs );

	QJsonArray volumes;

	for ( unsigned int i = 0; i != m_inputs; ++i )
	{
		QJsonArray data;

		for ( unsigned int j = 0; j != m_outputs; ++j )
		{
			data.push_back( ( double )m_volumes[i][j] );
		}

		volumes.push_back( data );
	}

	settings.insert( "volumes", volumes );
}

} /* namespace widgets */
} /* namespace noises */
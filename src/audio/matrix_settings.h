#pragma once

#include <QVector>

class QJsonObject;

namespace noises
{
namespace audio
{
	class MatrixSettings
	{
	public:
		MatrixSettings(
			unsigned int inputs = 2,
			unsigned int outputs = 2
			);

		// setting inputs and outputs causes
		// volume settings to be reset
		// TODO make optional?

		// could also provide set function that
		// does both in one go

		// inputs

		unsigned int getInputs() const;
		void setInputs( unsigned int inputs );

		// outputs

		unsigned int getOutputs() const;
		void setOutputs( unsigned int outputs );

		// matrix

		float getVolume(
			unsigned int input,
			unsigned int output ) const;

		void setVolume(
			unsigned int input,
			unsigned int output,
			float volume );

		// stereo link
		bool getStereoLink() const;
		void setStereoLink( bool value );

		// to determine mute/stopped state
		bool isMinimumVolume( float volume ) const;

		void readSettings( const QJsonObject& settings );

		void writeSettings( QJsonObject& settings ) const;

	private:
		void initialiseVolumes();

	private:
		unsigned int m_inputs, m_outputs;

		float m_minimum_value;
		QVector< QVector< float > > m_volumes;

		bool m_stereo_link;
	};

} /* namespace widgets */
} /* namespace noises */
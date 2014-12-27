#pragma once

#include <assert.h>

#include <QCheckBox>
#include <QLayout>
#include <QMap>
#include <QVector>
#include <QWidget>

namespace noises
{
namespace widgets
{
	class VolumeDial;

	// Matrix Settings

	class MatrixSettings
	{
	public:
		MatrixSettings();

		float volume(
			unsigned int input,
			unsigned int output );

		void setVolume(
			unsigned int input,
			unsigned int output,
			float volume );

		// to determine mute/stopped state
		bool isMinimumVolume( float volume );

	private:
		unsigned int m_inputs, m_outputs;

		float m_minimum_value;
		QVector< QVector< float > > m_volumes;

	};

	// Matrix Widget

	class Matrix: public QWidget
	{
		Q_OBJECT

	public:
		Matrix(
			unsigned int inputs = 2,
			unsigned int outputs = 2,
			QWidget* parent = 0 );

		~Matrix();

		void setInputs( unsigned int inputs );
		void setOutputs( unsigned int outputs );

		unsigned int getInputs() const;
		unsigned int getOutputs() const;

		float getValue( unsigned int input, unsigned int output ) const;
		void setValue( unsigned int input, unsigned int output, float value );

		void readSettings( const MatrixSettings& settings );
		void writeSettings( MatrixSettings& settings ) const;

	private Q_SLOTS:
		void valueChanged( float value );

	private:
		void createWidgets(
			unsigned int inputs,
			unsigned int outputs );

		void updateVolumeGrid(
			unsigned int inputs,
			unsigned int outputs );

		void clearVolumeGrid();

	private:
		struct IndexData
		{
			unsigned int input, output;
		};

		typedef QMap< VolumeDial*, IndexData > IndexDataMap;

		QGridLayout* m_volume_grid;
		QCheckBox* m_link;

		IndexDataMap m_index_map;
	};

} /* namespace widgets */
} /* namespace noises */
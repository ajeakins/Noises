#pragma once

#include <QMap>
#include <QWidget>

class QCheckBox;
class QGridLayout;

namespace noises
{
namespace audio
{
	class VolumeMatrix;
} /* namespace audio */
namespace widgets
{
	class VolumeDial;

	// Matrix Widget

	class Matrix: public QWidget
	{
		Q_OBJECT

	public:
		Matrix(
			unsigned int inputs = 2,
			unsigned int outputs = 2,
			QWidget* parent = nullptr );

		~Matrix();

		void setInputs( unsigned int inputs );
		void setOutputs( unsigned int outputs );

		unsigned int getInputs() const;
		unsigned int getOutputs() const;

		float getVolume( unsigned int input, unsigned int output ) const;
		void setVolume( unsigned int input, unsigned int output, float value );

		void readSettings( const audio::VolumeMatrix& settings );
		bool writeSettings( audio::VolumeMatrix& settings ) const;

	Q_SIGNALS:
		void volumeChanged();

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

		VolumeDial* getVolumeDial( unsigned int input, unsigned int output ) const;

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
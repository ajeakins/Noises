#pragma once

#include <QObject>
#include <QTime>
#include <QVector>

#include "player.h"

struct SF_INFO;

namespace noises
{
namespace audio
{
	class MatrixSettings;

	class AudioPlayer: public Player
	{
		Q_OBJECT
	public:
		typedef QSharedPointer< AudioPlayer > Ptr;

		~AudioPlayer() override;

		// setup

		void setFilename( const QString& filename );

		void setVolume( const MatrixSettings& settings );

		QTime getDuration() const;

		// Playback controls

		void start() override;

		void pause();

		void stop() override;

		void updateTime() const override;

	private:
		friend class Manager;
		friend class Engine;

		AudioPlayer( QObject* parent );

		float getVolume( int input, int output ) const;

		void readData();

		QTime timeFromFrames( int frames ) const;

		void addData(
			float* audio_data,
			int frames,
			int channels ) override;

	private:
		QString m_filename;

		// playback state
		int m_pos = 0 ;

		// audio data
		float* m_audio_data = nullptr;

		// audio information
		QSharedPointer< SF_INFO > m_audio_info;
		int m_length;

		// volume data
		QVector< QVector< float > > m_volumes;
		// current number of outputs we have data for
		int m_outputs;
	};

} /* namespace audio */
} /* namespace noises */

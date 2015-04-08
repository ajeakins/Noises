#pragma once

#include <QObject>
#include <QTime>

#include <portaudio.h>
#include <sndfile.h>

#include <widgets/matrix.h>

#include "player.h"

namespace noises
{
namespace audio
{
	class AudioPlayer: public Player
	{
		Q_OBJECT
	public:
		typedef QSharedPointer< AudioPlayer > Ptr;

		~AudioPlayer();

		// setup

		void setFilename( const QString& filename );

		void setVolume( const widgets::MatrixSettings& settings );

		QTime getDuration() const;

		// Playback controls

		void start();

		void pause();

		void stop();

		void updateTime();

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
			int channels );

	private:
		QString m_filename;

		// playback state
		int m_pos = 0 ;
		int m_last_pos = 0;

		// audio data
		float* m_audio_data = nullptr;

		// audio information
		SF_INFO m_audio_info;
		int m_length;

		// volume data
		QVector< QVector< float > > m_volumes;
		// current number of outputs we have data for
		int m_outputs;
	};

} /* namespace audio */
} /* namespace noises */

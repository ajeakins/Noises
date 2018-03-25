#pragma once

#include <QObject>
#include <QTime>
#include <QVector>

#include "fade_player.h"

#include "player.h"

struct SF_INFO;

namespace noises
{
namespace audio
{
	class VolumeMatrix;

	class AudioPlayer: public Player
	{
		Q_OBJECT
	public:
		typedef QSharedPointer< AudioPlayer > Ptr;

		~AudioPlayer() override;

		// setup
		void setFilename( const QString& filename );

		void setVolume( const VolumeMatrix& settings );

		QTime getDuration() const;

		void addFade( FadePlayer::Ptr fade_player );

		// playback controls
		void start() override;

		void pause();

		void stop() override;

		void updateTime() const override;

	Q_SIGNALS:
		void onError( const QString& title, const QString& message);

	public Q_SLOTS:
		void setPan( int pan ); // set pan as a percentage

	private:
		friend class Manager;
		friend class Engine;

		AudioPlayer();

		void readData();

		int timeInFrames() const;

		QTime timeFromFrames( int frames ) const;

		void addData(
			float* audio_data,
			int frames,
			int channels ) override;

		void resetVolumes();

		void updateCurrentVolumes();

	private:
		QString m_filename;

		// playback state
		int m_pos = 0 ;

		// audio

		// Data about the audio we read from the file
		QSharedPointer< SF_INFO > m_audio_info;

		// Data pointer
		float* m_audio_data = nullptr;

		// Data buffer length
		int m_length;

		// Audio frames held (potentially after resampling)
		int m_frames;

		// volumes
		VolumeMatrix m_volumes;

		// pan
		int m_pan;

		// Current volumes taking into account fades
		// converted to a multiplier
		typedef QVector< QVector< float > > Volumes;
		Volumes m_current_volumes;

		// fades
		struct FadeEntry
		{
			int start_frame;
			FadePlayer::Ptr fade;
			Volumes m_start_volumes;
		};
		typedef QList< FadeEntry > Fades;
		Fades m_fades;
	};

} /* namespace audio */
} /* namespace noises */

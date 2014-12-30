
#include "audio_cue_model_item.h"

namespace noises
{

AudioCueModelItem::AudioCueModelItem(
	const QList< QVariant >& item_data,
	CueModelItem* parent_item )
:
		CueModelItem( item_data, parent_item )
{}

AudioCueModelItem::~AudioCueModelItem()
{}

void AudioCueModelItem::execute() const
{
	std::cout << "play audio" << std::endl;
}

QVariant AudioCueModelItem::getIcon() const
{
	QPixmap icon( ":/images/audio_cue_16x16.png" );
	return icon;
}

} /* namespace noises */

#include <QApplication>
#include <QDebug>

#include <cues/cue_model_item.h>
#include <cues/audio_cue_model_item.h>

#include <utils/time.h>

#include "cue_model.h"

#include "progress_delegate.h"

namespace noises
{

void ProgressDelegate::paint(
	QPainter* painter,
	const QStyleOptionViewItem& option,
	const QModelIndex& index ) const
{
	QStyledItemDelegate::paint( painter, option, index );

	const CueModel* model = static_cast< const CueModel* >( index.model() );
	CueModelItem* item = model->itemFromIndex( index );

	if ( item->getType() == CueType_Audio )
	{
		AudioCueModelItem* audio_cue = static_cast< AudioCueModelItem* >( item );

		QString time_string = index.data().toString();
		QTime time = QTime::fromString( time_string, audio_cue->getTimeFormat() );

		QStyleOptionProgressBar progressBarOption;
		progressBarOption.rect = option.rect;
		progressBarOption.minimum = 0;
		progressBarOption.maximum = 100;
		progressBarOption.progress = utils::percentage( time, audio_cue->getDuration() );
		progressBarOption.text = time_string;
		progressBarOption.textVisible = true;

		QApplication::style()->drawControl(
			QStyle::CE_ProgressBar,
			&progressBarOption,
			painter);
	}
}

} /* namespace noises */

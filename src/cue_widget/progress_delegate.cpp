
#include <QApplication>
#include <QDebug>

#include <cues/cue_model_item.h>
#include <cues/audio_cue_model_item.h>
#include <cues/wait_cue_model_item.h>

#include <utils/time.h>

#include "cue_model.h"

#include "progress_delegate.h"

namespace noises
{

ProgressDelegate::ProgressDelegate( QObject* parent )
:
	QStyledItemDelegate( parent )
{}

void ProgressDelegate::paint(
	QPainter* painter,
	const QStyleOptionViewItem& option,
	const QModelIndex& index ) const
{
	QStyledItemDelegate::paint( painter, option, index );

	const CueModel* model = static_cast< const CueModel* >( index.model() );
	CueModelItem* item = model->itemFromIndex( index );

	// make this more generic...
	CueType type =  item->getType();
	if ( type == CueType_Audio || type == CueType_Wait )
	{
		QString time_string;
		float percentage = 0.0f;

		if ( type == CueType_Audio )
		{
			AudioCueModelItem* cue = static_cast< AudioCueModelItem* >( item );

			time_string = index.data().toString();
			QTime time = QTime::fromString( time_string, cue->getTimeFormat() );
			percentage = utils::percentage( time, cue->getDuration() );
		}
		else if ( type == CueType_Wait )
		{
			WaitCueModelItem* cue = static_cast< WaitCueModelItem* >( item );

			time_string = index.data().toString();
			QTime time = QTime::fromString( time_string, cue->getTimeFormat() );
			percentage = utils::percentage( time, cue->getDuration() );
		}

		QStyleOptionProgressBar progressBarOption;
		progressBarOption.rect = option.rect;
		progressBarOption.minimum = 0;
		progressBarOption.maximum = 100;
		progressBarOption.progress = percentage;
		progressBarOption.text = time_string;
		progressBarOption.textVisible = true;

		QApplication::style()->drawControl(
			QStyle::CE_ProgressBar,
			&progressBarOption,
			painter);
	}
}

} /* namespace noises */

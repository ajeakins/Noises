
#include <QApplication>

#include <cue/cue_model_item.h>

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

	if ( item->hasDuration() )
	{
		QTime time = index.data().toTime();
		float percentage = utils::percentage( time, item->getDuration() );
		QString time_string = time.toString( item->getTimeFormat() );

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

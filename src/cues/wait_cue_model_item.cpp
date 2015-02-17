
#include <QPixmap>

#include "wait_cue_model_item.h"

namespace noises
{

WaitCueModelItem::WaitCueModelItem(
	const QList< QVariant >& item_data,
	CueModelItem* parent_item )
:
		CueModelItem( item_data, parent_item )
{}

WaitCueModelItem::~WaitCueModelItem()
{}

void WaitCueModelItem::execute() const
{}

QVariant WaitCueModelItem::getIcon() const
{
	QPixmap icon( ":/images/wait_cue_16x16.png" );
	return icon;
}

} /* namespace noises */

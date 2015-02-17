
#include <QPixmap>

#include "control_cue_model_item.h"

namespace noises
{

ControlCueModelItem::ControlCueModelItem(
	const QList< QVariant >& item_data,
	CueModelItem* parent_item )
:
		CueModelItem( item_data, parent_item )
{}

ControlCueModelItem::~ControlCueModelItem()
{}

void ControlCueModelItem::execute() const
{}

QVariant ControlCueModelItem::getIcon() const
{
	QPixmap icon( ":/images/control_cue_16x16.png" );
	return icon;
}

} /* namespace noises */

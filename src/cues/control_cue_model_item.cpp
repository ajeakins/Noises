
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
{
	std::cout << "control cue" << std::endl;
}

} /* namespace noises */

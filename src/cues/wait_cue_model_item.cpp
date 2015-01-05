
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
{
	std::cout << "wait cue" << std::endl;
}

} /* namespace noises */

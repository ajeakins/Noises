
#include "group_cue_model_item.h"

namespace noises
{

GroupCueModelItem::GroupCueModelItem(
	const QList< QVariant >& item_data,
	CueModelItem* parent_item )
:
		CueModelItem( item_data, parent_item )
{}

GroupCueModelItem::~GroupCueModelItem()
{}

void GroupCueModelItem::execute() const
{
	std::cout << "group cue" << std::endl;
}

Qt::ItemFlags GroupCueModelItem::flags() const
{
	Qt::ItemFlags flags = CueModelItem::flags();
	flags |= Qt::ItemIsDropEnabled;
	return flags;
}

} /* namespace noises */

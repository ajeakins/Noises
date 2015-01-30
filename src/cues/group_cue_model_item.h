#pragma once

#include <iostream>

#include "cue_model_item.h"
#include "types.h"

namespace noises
{
	class GroupCueModelItem: public CueModelItem
	{
	public:
		GroupCueModelItem(
			const QList< QVariant >& item_data,
			CueModelItem* parent_item = 0 );

		virtual ~GroupCueModelItem();

		virtual CueType getType() const
		{
			return CueType_Group;
		}

		void execute() const;

		Qt::ItemFlags flags() const;

	protected:
		QVariant getIcon() const;

	private:
		// settings
	};

} /* namespace noises */

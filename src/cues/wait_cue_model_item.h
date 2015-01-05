#pragma once

#include <iostream>

#include "cue_model_item.h"
#include "types.h"

namespace noises
{
	class WaitCueModelItem: public CueModelItem
	{
	public:
		WaitCueModelItem(
			const QList< QVariant >& item_data,
			CueModelItem* parent_item = 0 );

		virtual ~WaitCueModelItem();

		virtual CueType getType() const
		{
			return CueType_Wait;
		}

		void execute() const;

	private:
		// settings
	};

} /* namespace noises */

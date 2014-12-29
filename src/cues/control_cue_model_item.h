#pragma once

#include <iostream>

#include "cue_model_item.h"
#include "types.h"

namespace noises
{
	class ControlCueModelItem: public CueModelItem
	{
	public:
		ControlCueModelItem(
			const QList< QVariant >& item_data,
			CueModelItem* parent_item = 0 );

		virtual ~ControlCueModelItem();

		virtual CueType getType() const
		{
			return CueType_Control;
		}

		void execute() const;

	private:
		// settings
	};

} /* namespace noises */

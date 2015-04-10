#pragma once

#include "cue_model_item.h"

namespace noises
{
	class ControlCueModelItem: public CueModelItem
	{
	public:
		ControlCueModelItem(
			const QList< QVariant >& item_data,
			CueModelItem* parent_item = nullptr );

		~ControlCueModelItem();

		CueType getType() const
		{
			return CueType_Control;
		}

		void execute();

	protected:
		QVariant getIcon() const;

	private:

	};

} /* namespace noises */

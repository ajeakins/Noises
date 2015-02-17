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

		virtual ~ControlCueModelItem();

		virtual CueType getType() const
		{
			return CueType_Control;
		}

		void execute() const;

	protected:
		QVariant getIcon() const;

	private:

	};

} /* namespace noises */

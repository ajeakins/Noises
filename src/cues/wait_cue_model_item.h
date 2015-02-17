#pragma once

#include "cue_model_item.h"

namespace noises
{
	class WaitCueModelItem: public CueModelItem
	{
	public:
		WaitCueModelItem(
			const QList< QVariant >& item_data,
			CueModelItem* parent_item = nullptr );

		virtual ~WaitCueModelItem();

		virtual CueType getType() const
		{
			return CueType_Wait;
		}

		void execute() const;

	protected:
		QVariant getIcon() const;

	private:
		// settings
	};

} /* namespace noises */

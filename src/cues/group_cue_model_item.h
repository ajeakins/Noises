#pragma once

#include "cue_model_item.h"

namespace noises
{
	class GroupCueModelItem: public CueModelItem
	{
	public:
		GroupCueModelItem(
			const QList< QVariant >& item_data,
			CueModelItem* parent_item = nullptr );

		~GroupCueModelItem();

		CueType getType() const override
		{
			return CueType_Group;
		}

		void execute() override;

		Qt::ItemFlags flags() const override;

	protected:
		QVariant getIcon() const override;

	private:
		// settings
	};

} /* namespace noises */

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

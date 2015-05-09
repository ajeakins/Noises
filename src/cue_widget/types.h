#pragma once

#include <QString>

#include <utils/macro.h>

namespace noises
{
	enum Column
	{
		Column_Cue,
		Column_Description,
		Column_Notes,
		Column_Remaining,
		Column_Elapsed,
		Column_PostAction,

		Column_ITEM_COUNT
	};

	NOISES_DECLARE_ITERABLE_ENUM( Column )

	QString columnToString( Column column );

	static const Column editableColumns[] = {Column_Cue};

} /* namespace noises */

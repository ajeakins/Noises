#pragma once

#include <QString>

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

		ColumnCount
	};

	inline Column& operator++( Column& column )
	{
		column = (Column)((int)column + 1);
		return column;
	}

	QString columnToString( Column column );

	static const Column editableColumns[] = {Column_Cue};

} /* namespace noises */

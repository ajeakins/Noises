
#include "types.h"

namespace noises
{

QString columnToString( Column column )
{
	switch( column )
	{
		case Column_Cue:
			return "Cue";
		case Column_Description:
			return "Description";
		case Column_Notes:
			return "Notes";
		case Column_Remaining:
			return "Remaining";
		case Column_Elapsed:
			return "Elapsed";
		case Column_PostAction:
			return "Post Action";
		case Column_ITEM_COUNT:
			return "";
	}

	return "";
}

} /* namespace noises */
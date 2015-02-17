
#include <assert.h>

#include "types.h"

namespace noises
{
	QString typeToString( CueType type )
	{
		switch( type )
		{
		case CueType_Audio:
			return "Audio";
		case CueType_Control:
			return "Control";
		case CueType_Wait:
			return "Wait";
		case CueType_Group:
			return "Group";
		case CueType_None:
			return "None";
		}

		return "";
	}

	CueType stringToType( const QString& type )
	{
		if ( type == "Audio" )
		{
			return CueType_Audio;
		}
		else if( type == "Control" )
		{
			return CueType_Control;
		}
		else if ( type == "Wait" )
		{
			return CueType_Wait;
		}
		else if ( type == "Group" )
		{
			return CueType_Group;
		}

		assert( false && "Unhandled cue type" );
		return CueType_None;
	}

} /* noises */

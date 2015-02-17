#pragma once

#include <QString>

namespace noises
{
	enum CueType
	{
		CueType_None,		// invalid cues i.e. the header model item
		CueType_Audio,		// single audio clip
		CueType_Control,	// acts on another cue to change parameters
		CueType_Wait,		// delay execution of the next cue
		CueType_Group		// group of cues
	};

	QString typeToString( CueType type );

	CueType stringToType( const QString& type );

} /* noises */

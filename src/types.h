#pragma once

namespace noises
{
	enum CueType
	{
		CueType_None,		// invalid cues i.e. the header model item
		CueType_Audio,		// single audio clip
		CueType_Control		// acts on another cue to change parameters
	};

} /* noises */
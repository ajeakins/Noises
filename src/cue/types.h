#pragma once

#include <QString>
#include <QFlags>

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

	enum PostAction
	{
		PostAction_Advance			= 1 << 0,
		PostAction_AdvanceAndPlay	= 1 << 1,

		PostAction_ITEM_COUNT
	};

	Q_DECLARE_FLAGS(PostActions, PostAction)

	inline PostAction& operator++( PostAction& action )
	{
		action = (PostAction)((int)action + 1);
		return action;
	}

	QString postActionToString( PostAction action );

	PostAction stringToPostAction( const QString& action );

} /* noises */

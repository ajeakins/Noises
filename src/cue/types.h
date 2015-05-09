#pragma once

#include <QString>
#include <QFlags>

#include <utils/macro.h>

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

	Q_DECLARE_FLAGS( PostActions, PostAction )

	NOISES_DECLARE_ITERABLE_ENUM( PostAction )

	QString postActionToString( PostAction action );

	PostAction stringToPostAction( const QString& action );

} /* noises */

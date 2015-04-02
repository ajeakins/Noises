
#include <QDebug>

#include <cues/cue_model_item.h>

#include "audio_cue_dialog.h"
#include "control_cue_dialog.h"
#include "group_cue_dialog.h"
#include "wait_cue_dialog.h"

#include "api.h"

namespace noises
{

void showCueEditDialog(
	CueModelItem* item,
	QDataWidgetMapper* mapper,
	bool create,
	QWidget* parent )
{
	switch( item->getType() )
	{
		case CueType_Audio:
		{
			AudioCueDialog dialog(
				( AudioCueModelItem* )item,
				mapper,
				create,
				parent );
			dialog.exec();
			break;
		}
		case CueType_Control:
		{
			ControlCueDialog dialog(
				( ControlCueModelItem* )item,
				mapper,
				create,
				parent );
			dialog.exec();
			break;
		}
		case CueType_Wait:
		{
			WaitCueDialog dialog(
				( WaitCueModelItem* )item,
				mapper,
				create,
				parent );
			dialog.exec();
			break;
		}
		case CueType_Group:
		{
			GroupCueDialog dialog(
				( GroupCueModelItem* )item,
				mapper,
				create,
				parent );
			dialog.exec();
			break;
		}
		default:
			qWarning() << "WARNING: Unknown cue type\n";
			break;
	}

}

} /* namespace noises */

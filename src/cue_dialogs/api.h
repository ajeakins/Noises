#pragma once

class QDataWidgetMapper;
class QWidget;

namespace noises
{
	class CueModelItem;

	void showCueEditDialog(
		CueModelItem* item,
		QDataWidgetMapper* mapper,
		bool create,
		QWidget* parent );

} /* namespace noises */

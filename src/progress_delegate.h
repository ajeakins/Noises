#pragma once

#include <QStyledItemDelegate>

namespace noises
{
	class ProgressDelegate: public QStyledItemDelegate
	{
		void paint(
			QPainter* painter,
			const QStyleOptionViewItem& option,
			const QModelIndex& index ) const;

	};

} /* namespace noises */

#pragma once

#include <QStyledItemDelegate>

namespace noises
{
	class ProgressDelegate: public QStyledItemDelegate
	{
	public:
		ProgressDelegate( QObject* parent = nullptr );

		void paint(
			QPainter* painter,
			const QStyleOptionViewItem& option,
			const QModelIndex& index ) const;
	};

} /* namespace noises */

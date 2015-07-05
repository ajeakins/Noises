#pragma once

#include <QLineEdit>

// Line edit that expands to fit it's contents

namespace noises
{
namespace widgets
{
	class ExpandingLineEdit: public QLineEdit
	{
		Q_OBJECT
	public:
		ExpandingLineEdit( QWidget* parent = nullptr );

	private slots:
		void resizeToContents();
	};

} /* namespace widgets */
} /* namespace noises */

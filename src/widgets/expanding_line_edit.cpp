
#include "expanding_line_edit.h"

namespace noises
{
namespace widgets
{

ExpandingLineEdit::ExpandingLineEdit( QWidget* parent )
:
	QLineEdit( parent )
{
	connect(
		this, SIGNAL(textChanged(QString)),
		this, SLOT(resizeToContents()));
}

void ExpandingLineEdit::resizeToContents()
{
    int w = fontMetrics().width( text() );
    w += 20; // Font metrics are coming back a bit small...
    setMinimumSize( w, height() );
}

} /* namespace widgets */
} /* namespace noises */

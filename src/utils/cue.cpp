
#include "cue.h"

namespace noises
{
namespace utils
{

void getNewCueNumber(
	const QString& previous_number,
	const QString& next_number,
	QString& new_number )
{
	if ( previous_number.isEmpty() )
	{
		new_number = "1";
	}
	else if ( next_number.isEmpty() )
	{
		int previous = previous_number.toInt();
		new_number.setNum( previous + 1 );
	}
	else
	{
		float previous = previous_number.toFloat();
		float next = next_number.toFloat();

		if ( previous + 1 < next )
		{
			new_number.setNum( ( int )( previous + 1 ) );
		}
		else
		{
			new_number.setNum( previous + 0.1 );
		}
	}
}

} /* namspace noises */
} /* namespace utils */

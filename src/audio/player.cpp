
#include <assert.h>

#include "player.h"

namespace noises
{
namespace audio
{

Player::Player( QObject* parent )
:
	QObject(), // don't ask parent to manage lifetime
	m_parent( parent )
{
	assert( parent );

	connect(
		parent, &QObject::destroyed,
		[this](){ Q_EMIT parentDestroyed( sharedFromThis() ); } );
}

Player::~Player()
{}

} /* namespace audio */
} /* namespace noises */

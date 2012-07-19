/*
-----------------------------------------------------------------------------
Copyright (c) 2006-2012 Catalin Alexandru Nastase

KG game engine (http://k-game.sourceforge.net) is made available under the LGPL License (http://www.gnu.org/copyleft/lgpl.html)

Under the LGPL you may use KG game engine for any purpose you wish, as long as you:
1. Release any modifications to the KG game engine source back to the community
2. Pass on the source to KG game engine with all the copyrights intact, or link back to a place where the source code can be obtained (http://k-game.sourceforge.net)
3. Make it clear where you have customised it.
The above is a precis, please do read the full license agreement.
-----------------------------------------------------------------------------
*/

#include <input/Cursor.h>

namespace input
{

Cursor::Cursor()
{
	mAutoCenter = false;

	mPosition = core::position2d::ORIGIN_2D;
	mAbsolutePosition = core::position2d::ORIGIN_2D;
}

Cursor::~Cursor() {}

void Cursor::setAutoCenter(bool set)
{
	mAutoCenter = set;
}

bool Cursor::isAutoCenter()
{
	return mAutoCenter;
}

const core::position2d& Cursor::getPosition() const
{
	return mPosition;
}

const core::position2d& Cursor::getAbsolutePosition() const
{
	return mAbsolutePosition;
}

void Cursor::update(float elapsedTime)
{
	updateImpl(elapsedTime);
}

void Cursor::updateImpl(float elapsedTime) {}

} // end namespace input

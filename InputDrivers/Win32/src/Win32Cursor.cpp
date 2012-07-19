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

#include <core/Vector2d.h>
#include <Win32Cursor.h>

namespace input
{

Win32Cursor::Win32Cursor()
{
}

Win32Cursor::~Win32Cursor()
{
}

void Win32Cursor::setVisible(bool set)
{
	oi_app_cursor(set && !mAutoCenter ? OI_ENABLE : OI_DISABLE);
}

void Win32Cursor::setAutoCenter(bool set)
{
	Cursor::setAutoCenter(set);
	oi_app_cursor(set ? OI_DISABLE : OI_ENABLE);
}

void Win32Cursor::setPosition(const core::position2d& pos)
{
	setPosition(pos.X, pos.Y);
}

void Win32Cursor::setPosition(signed int x, signed int y)
{
	oi_mouse_warp(0, x, y);
}

void Win32Cursor::updateImpl(float elapsedTime)
{
	oi_mouse_relative(0, &mPosition.X, &mPosition.Y);

	oi_mouse_absolute(0, &mAbsolutePosition.X, &mAbsolutePosition.Y);
}

} // end namespace input

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

#ifndef _CURSOR_H_
#define _CURSOR_H_

#include <core/Config.h>
#include <core/Position2d.h>

namespace input
{

//! Interface to the mouse cursor.
class ENGINE_PUBLIC_EXPORT Cursor
{
public:

	Cursor();

	// destructor
	virtual ~Cursor();

	//! Changes the visible state of the mouse cursor.
	virtual void setVisible(bool set) = 0;

	//! Changes the auto-center state of the mouse cursor.
	virtual void setAutoCenter(bool set);

	//! Returns if the cursor is currently auto-center.
	virtual bool isAutoCenter();

	//! Sets the new position of the cursor.
	//! \param pos: New position of the cursor.
	virtual void setPosition(const core::position2d& pos) = 0;

	//! Sets the new position of the cursor.
	//! \param x: New X position of the cursor.
	//! \param y: New Y position of the cursor.
	virtual void setPosition(signed int x, signed int y) = 0;

	//! Returns the current position of the mouse cursor.
	//! \return Returns the current position of the cursor.
	const core::position2d& getPosition() const;

	//! Returns the current position of the mouse cursor.
	//! \return Returns the current position of the cursor.
	const core::position2d& getAbsolutePosition() const;

	void update(float elapsedTime);

public:

	virtual void updateImpl(float elapsedTime);

	bool mAutoCenter;

	core::position2d mPosition;
	core::position2d mAbsolutePosition;
};

} // end namespace input

#endif
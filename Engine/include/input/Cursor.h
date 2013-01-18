/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2013 Catalin Alexandru Nastase

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
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
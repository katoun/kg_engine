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

#ifndef _WIN32_CURSOR_H_
#define _WIN32_CURSOR_H_

#include <Win32Config.h>
#include <input/Cursor.h>

namespace input
{

class Win32Cursor : public Cursor
{
public:

	Win32Cursor();

	~Win32Cursor();

	void setVisible(bool set);

	void setAutoCenter(bool reset);

	void setPosition(const core::position2d& pos);

	void setPosition(signed int x, signed int y);

	const core::position2d& getPosition() const;

	const core::position2d& getAbsolutePosition() const;
	
private:

	void updateImpl(float elapsedTime);
};

} // end namespace input

#endif
/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2012 Catalin Alexandru Nastase

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

#include <input/Joystick.h>
#include <input/JoystickEvent.h>

namespace input
{

Joystick::Joystick()
{
	mInputType = INPUT_TYPE_JOYSTICK;
	
	mNumButtons = 0;
	mNumAxes = 0;
	mNumPOVs = 0;
	mEvent = new JoystickEvent();
}

Joystick::~Joystick()
{
	delete mEvent;
}

signed short int Joystick::getNumButtons()
{
	return mNumButtons;
}

signed short int Joystick::getNumAxes()
{
	return mNumAxes;
}

signed short int Joystick::getNumPOVs()
{
	return mNumPOVs;
}

void Joystick::addJoystickEventReceiver(input::JoystickEventReceiver* newEventReceiver)
{
	mJoystickEventReceivers.push_back(newEventReceiver);
}

void Joystick::removeJoystickEventReceiver(input::JoystickEventReceiver* oldEventReceiver)
{
	std::list<input::JoystickEventReceiver*>::iterator i;
	for (i = mJoystickEventReceivers.begin(); i != mJoystickEventReceivers.end(); ++i)
	{
		if ((*i) == oldEventReceiver)
		{
			mJoystickEventReceivers.erase(i);
			return;
		}
	}
}

} // end namespace input

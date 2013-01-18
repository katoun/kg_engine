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

#include <Win32Config.h>
#include <input/InputManager.h>
#include <Win32InputDriver.h>

namespace input
{

InputDriver* win32InputDriver = nullptr;

extern "C" void WININPUT_PUBLIC_EXPORT loadPlugin()
{
	win32InputDriver = new Win32InputDriver();
	if (InputManager::getInstance() != nullptr)
		InputManager::getInstance()->setSystemDriver(win32InputDriver);
}

/// Destroys
extern "C" void WININPUT_PUBLIC_EXPORT unloadPlugin()
{
	if (InputManager::getInstance() != nullptr)
		InputManager::getInstance()->removeSystemDriver();
	SAFE_DELETE(win32InputDriver);
}

} // end namespace input

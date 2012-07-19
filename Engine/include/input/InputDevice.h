/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

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

#ifndef _INPUT_DEVICE_H_
#define _INPUT_DEVICE_H_

#include <core/Config.h>
#include <input/InputDeviceDefines.h>

#include <string>

namespace input
{

//! A common base class for input devices.
class ENGINE_PUBLIC_EXPORT InputDevice
{
public:

	//! Constructor
	InputDevice();

	//! Destructor
	virtual ~InputDevice();

	//! Get the type of device.
	InputType getInputType();

	//! Get buffered mode - true is buffered, false otherwise.
	bool getBuffered();

	//! Sets buffered mode.
	void setBuffered(bool buffered);

	//! Get the device vendor.
	const std::string& getVendor() const;

	//! Initialize input device.
	void initialize();

	//! Uninitialize input device.
	void uninitialize();

	void update(float elapsedTime);

protected:

	virtual void initializeImpl();
	virtual void uninitializeImpl();
	virtual void updateImpl(float elapsedTime);

	// Type of controller object
	InputType mInputType;

	// Buffered flag
	bool mBuffered;

	std::string mVendor;
};

} // end namespace input

#endif
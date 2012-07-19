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
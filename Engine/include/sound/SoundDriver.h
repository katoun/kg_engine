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

#ifndef _SOUND_DRIVER_H_
#define _SOUND_DRIVER_H_

#include <core/Config.h>
#include <core/SystemDriver.h>

namespace core
{
class vector3d;
class quaternion;
}

namespace sound
{

class Sound;
class SoundData;
class Listener;

//! Defines the functionality of a 3D Sound API
//!
//! The SoundSystem class provides a base interface
//! which abstracts the general functionality of the 3D API
//! e.g. OpenAL or Fmod. Whilst a few of the general
//! methods have implementations, most of this class is
//! abstract, requiring a subclass based on a specific API
//! to be constructed to provide the full functionality.
class ENGINE_PUBLIC_EXPORT SoundDriver : core::SystemDriver
{
public:

	// Default Constructor
	SoundDriver(const std::string& name);

	// Destructor
	virtual ~SoundDriver();

	virtual void updateListener(Listener* listener) = 0;
	
	virtual void setDopplerFactor(float dopplerFactor) = 0;

	virtual void setSoundSpeed(float soundSpeed) = 0;
};

} // end namespace sound

#endif
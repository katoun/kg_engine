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
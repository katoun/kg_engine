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

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <EngineConfig.h>
#include <Core.h>
#include <Platform.h>
#include <Input.h>
#include <Resource.h>
#include <Game.h>
#include <Render.h>
#include <Sound.h>
#include <Physics.h>

#include <engine/Plugin.h>
#include <engine/PluginManager.h>
#include <engine/EngineSettings.h>
#include <engine/EngineManager.h>
#include <engine/EngineEvent.h>
#include <engine/EngineEventReceiver.h>

//! \mainpage KG Game Engine 0.8.0 API documentation
//!
//! <div align="center"><img src="kgenginelogo.png" ></div>
//!
//! \section intro Introduction
//! Welcome to the KG Game Engine API documentation.
//! Here you'll find any information you'll need to develop applications with
//! this Game Engine.
//! <BR>

// THE FOLLOWING IS AN EMPTY LIST OF ALL SUB NAMESPACES
// EXISTING ONLY FOR THE DOCUMENTION SOFTWARE DOXYGEN.

//! In this namespace can be found basic classes like vectors, planes, arrays, lists and so on.
namespace core
{
}
//! In this namespace can be found game management classes.
namespace game
{
}
//! In this namespace can be found platform management classes like Timer, File, and so on.
namespace platform
{
}
//! In this namespace can be found input management classes like MouseEvent, MouseEventReceiver, KeyEvent and so on.
namespace input
{
}
//! In this namespace can be found resource management classes like ResoureceManager, Resource and so on.
namespace resource
{
}
//! In this namespace can be found render management classes like RenderManager, RenderWindow, RenderSystem, and so on.
namespace render
{
}
//! In this namespace can be found sound management classes.
namespace sound
{
}
//! In this namespace can be found physics management classes.
namespace physics
{
}
//! In this namespace can be found engine management classes.
namespace engine
{
}

#endif // _ENGINE_H_
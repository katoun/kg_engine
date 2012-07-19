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

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <core/Config.h>
#include <Core.h>
#include <Platform.h>
#include <Input.h>
#include <Resource.h>
#include <Game.h>
#include <Render.h>
#include <Sound.h>
#include <Physics.h>
#include <Scene.h>

#include <engine/Object.h>
#include <engine/ObjectDefines.h>
#include <engine/Plugin.h>
#include <engine/PluginManager.h>
#include <engine/EngineSettings.h>
#include <engine/EngineManager.h>
#include <engine/EngineEvent.h>
#include <engine/EngineEventReceiver.h>

//! \mainpage KG Game Engine
//!
//! <div align="center"><img src="kgenginelogo.png" ></div>
//!
//! \section intro Introduction
//! Welcome to the KG Game Engine API documentation.
//! Here you'll find any information you'll need to develop applications with
//! this Game Engine.
//!
//! \section links Links
//!
//! <A HREF="namespaces.html">Namespaces</A>: A very good place to start reading the documentation.<BR>
//! <A HREF="annotated.html">Compound list</A>: List of all classes with descriptions.<BR>
//! <A HREF="hierarchy.html">Class hierarchy</A>: Class hierarchy list.<BR>
//! <A HREF="classes.html">Alphabetical class list</A>: Good place to find forgotten class names.<BR>
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
//! In this namespace can be found scene management classes like SceneManager, SceneNode, Camera, and so on.
namespace scene
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
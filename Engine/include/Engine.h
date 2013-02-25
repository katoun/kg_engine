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

#include <core/Singleton.h>
#include <core/Log.h>
#include <core/LogDefines.h>
#include <core/System.h>
#include <core/SystemDefines.h>
#include <core/SystemDriver.h>

#include <core/Math.h>
#include <core/Utils.h>

#include <core/Aabox3d.h>
#include <core/Plane3d.h>
#include <core/Sphere3d.h>

#include <platform/PlatformDefines.h>
#include <platform/PlatformManager.h>

#include <input/Cursor.h>
#include <input/InputEvent.h>
#include <input/InputDevice.h>
#include <input/InputDeviceDefines.h>
#include <input/Mouse.h>
#include <input/MouseDefines.h>
#include <input/MouseEvent.h>
#include <input/MouseEventReceiver.h>
#include <input/Keyboard.h>
#include <input/KeyboardDefines.h>
#include <input/KeyEvent.h>
#include <input/KeyEventReceiver.h>
#include <input/Joystick.h>
#include <input/JoystickDefines.h>
#include <input/JoystickEvent.h>
#include <input/JoystickEventReceiver.h>
#include <input/InputDriver.h>
#include <input/InputManager.h>

#include <resource/Resource.h>
#include <resource/ResourceDefines.h>
#include <resource/ResourceEvent.h>
#include <resource/ResourceEventReceiver.h>
#include <resource/PixelFormat.h>
#include <resource/Serializer.h>
#include <resource/LoadEvent.h>
#include <resource/LoadEventReceiver.h>
#include <resource/ResourceManager.h>

#include <game/GameObject.h>
#include <game/Component.h>
#include <game/ComponentDefines.h>
#include <game/ComponentFactory.h>
#include <game/Transform.h>
#include <game/TransformDefines.h>
#include <game/MessageDefines.h>
#include <game/Scene.h>
#include <game/SceneFactory.h>
#include <game/GameManager.h>

#include <render/RenderUtils.h>
#include <render/BufferDefines.h>
#include <render/IndexBuffer.h>
#include <render/IndexBufferDefines.h>
#include <render/VertexBuffer.h>
#include <render/VertexBufferDefines.h>
#include <render/RenderDefines.h>
#include <render/Camera.h>
#include <render/CameraDefines.h>
#include <render/Frustum.h>
#include <render/FrustumDefines.h>
#include <render/Light.h>
#include <render/LightDefines.h>
#include <render/Font.h>
#include <render/FontFactory.h>
#include <render/Model.h>
#include <render/MeshData.h>
#include <render/MeshDataFactory.h>
#include <render/Material.h>
#include <render/Texture.h>
#include <render/TextureDefines.h>
#include <render/Shader.h>
#include <render/ShaderDefines.h>
#include <render/ShaderParameter.h>
#include <render/ShaderParameterDefines.h>
#include <render/RenderStateData.h>
#include <render/Viewport.h>
#include <render/RenderTargetEvent.h>
#include <render/RenderTargetEventReceiver.h>
#include <render/RenderTarget.h>
#include <render/RenderWindow.h>
#include <render/FrameEventReceiver.h>
#include <render/RenderManager.h>

#include <sound/Sound.h>
#include <sound/SoundUtils.h>
#include <sound/SoundData.h>
#include <sound/Listener.h>
#include <sound/SoundFactory.h>
#include <sound/SoundManager.h>

#include <physics/Body.h>
#include <physics/BodyFactory.h>
#include <physics/BodyData.h>
#include <physics/BodyDataFactory.h>
#include <physics/Material.h>
#include <physics/Shape.h>
#include <physics/ShapeDefines.h>
#include <physics/Constraint.h>
#include <physics/ConstraintDefines.h>
#include <physics/CollisionPoint.h>
#include <physics/CollisionEvent.h>
#include <physics/CollisionEventReceiver.h>
#include <physics/PhysicsManager.h>

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
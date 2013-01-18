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

#ifndef _SCENE_SERIALIZER_H_
#define _SCENE_SERIALIZER_H_

#include <Config.h>
#include <core/Utils.h>
#include <resource/Serializer.h>

#include <string>

#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>

namespace game
{
class GameObject;
}

namespace resource
{

//! Class for serialising scene data.
class SceneSerializer: public Serializer
{
public:

	SceneSerializer();
	virtual ~SceneSerializer();

	//! Imports a Scene from an .xml file.
	bool importResource(Resource* dest, const std::string& filename);

	//! Exports a Scene to the file specified. 
	//!
	//! This method takes an externally created Scene, and exports it to a .xml file.
	//! \param meshData: Pointer to the MeshData to export
	//! \param filename: The destination filename.
	bool exportResource(Resource* source, const std::string& filename);

private:

	void importGameObjects(Poco::AutoPtr<Poco::Util::XMLConfiguration>& config, std::string rootKey = "", game::GameObject* parent = nullptr);
};

}// end namespace resource

#endif
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

#ifndef _SPHERE_3D_H_
#define _SPHERE_3D_H_

#include <EngineConfig.h>

#include <glm/glm.hpp>

namespace core
{

//! Sphere
class ENGINE_PUBLIC_EXPORT sphere3d
{
public:

	// Constructors

	sphere3d();
	sphere3d(const glm::vec3& center, const float radius);
	sphere3d(const sphere3d& other);

	// operators

	sphere3d& operator=(const sphere3d& other);

	bool operator==(const sphere3d& other) const;
	bool operator!=(const sphere3d& other) const;

	// functions

	inline void set(const glm::vec3 center, const float radius);
	inline void set(const sphere3d& s);

	glm::vec3 Center;
	float Radius;
};

} // end namespace core

#endif
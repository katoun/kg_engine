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

#ifndef _LINE_3D_H_
#define _LINE_3D_H_

#include <EngineConfig.h>
#include <core/Vector3d.h>

namespace core
{

//! 3D line between two points with intersection methods.
class ENGINE_PUBLIC_EXPORT line3d
{
public:

	line3d();
	line3d(float xa, float ya, float za, float xb, float yb, float zb);
	line3d(const vector3d& nstart, const vector3d& nend);
	line3d(const line3d& other);

	line3d operator+(const vector3d& point) const;
	line3d& operator+=(const vector3d& point);

	line3d operator-(const vector3d& point) const;
	line3d& operator-=(const vector3d& point);

	bool operator==(const line3d& other) const;
	bool operator!=(const line3d& other) const;

	void set(const float& xa, const float& ya, const float& za, const float& xb, const float& yb, const float& zb);
	void set(const vector3d& nstart, const vector3d& nend);
	void set(const line3d& other);

	//! Returns length of line
	//! \return Returns length of line.
	float getLength() const;

	//! Returns sqared length of line
	//! \return Returns sqared length of line.
	float getLengthSQ() const;

	//! Returns middle of line
	vector3d getMiddle() const;

	//! Returns vector of line
	vector3d getVector() const;

	//! Returns if the over given point is between start and end of the
	//! line. Assumes that the point is already somewhere on the line.
	bool isPointBetweenStartAndEnd(const vector3d& point) const;

	//! Returns the closest point on this line to a point
	vector3d getClosestPoint(const vector3d& point) const;

	// member variables
	vector3d start;
	vector3d end;
};

} // end namespace core

#endif
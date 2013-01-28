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

#ifndef _LINE_2D_H_
#define _LINE_2D_H_

#include <Config.h>
#include <core/Vector2d.h>

namespace core
{

//! 2D line between two points with intersection methods.
class ENGINE_PUBLIC_EXPORT line2d
{
public:

	line2d();
	line2d(float xa, float ya, float xb, float yb);
	line2d(const vector2d& nstart, const vector2d& nend);
	line2d(const line2d& other);

	line2d operator+(const vector2d& point) const;
	line2d& operator+=(const vector2d& point);

	line2d operator-(const vector2d& point) const;
	line2d& operator-=(const vector2d& point);

	bool operator==(const line2d& other) const;
	bool operator!=(const line2d& other) const;

	void set(const float& xa, const float& ya, const float& xb, const float& yb);
	void set(const vector2d& nstart, const vector2d& nend);
	void set(const line2d& other);

	//! Returns length of line
	//! \return Returns length of line.
	float getLength() const;

	//! Returns the vector of the line.
	//! \return Returns the vector of the line.
	vector2d getVector() const;

	//! Returns unit vector of the line.
	//! \return Returns unit vector of this line.
	vector2d getUnitVector();

	float getAngleWith(const line2d& l);

	vector2d start;
	vector2d end;
};

} // end namespace core

#endif
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

#ifndef _LINE_2D_H_
#define _LINE_2D_H_

#include <core/Config.h>
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
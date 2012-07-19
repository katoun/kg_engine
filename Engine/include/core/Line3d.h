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

#ifndef _LINE_3D_H_
#define _LINE_3D_H_

#include <core/Config.h>
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
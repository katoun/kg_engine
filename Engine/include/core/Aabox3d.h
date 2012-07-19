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

#ifndef _AABBOX_3D_H_
#define _AABBOX_3D_H_

#include <core/Config.h>
#include <core/Vector3d.h>

namespace core
{

//! Axis aligned box in 3d dimensional space.
//!  Has some useful methods used with occlusion culling or clipping.
class ENGINE_PUBLIC_EXPORT aabox3d
{
public:

	aabox3d();
	aabox3d(const vector3d& min, const vector3d& max);
	aabox3d(const vector3d& init);
	aabox3d(float minx, float miny, float minz, float maxx, float maxy, float maxz);

	inline bool operator==(const aabox3d& other) const;
	inline bool operator!=(const aabox3d& other) const;

	//! Adds a point to the bounding box, causing it to grow bigger,
	//! if point is outside of the box.
	//! \param x: X Coordinate of the point to add to this box.
	//! \param y: Y Coordinate of the point to add to this box.
	//! \param z: Z Coordinate of the point to add to this box.
	void addInternalPoint(float x, float y, float z);

	//! Adds a point to the bounding box, causing it to grow bigger,
	//! if point is outside of the box
	//! \param p: Point to add into the box.
	void addInternalPoint(const vector3d& p);

	//! Adds an other bounding box to the bounding box, causing it to grow bigger,
	//! if the box is outside of the box
	//! \param b: Other bounding box to add into this box.
	void addInternalBox(const aabox3d& b);

	//! Resets the bounding box.
	void reset(float x, float y, float z);

	//! Resets the bounding box.
	void reset(const aabox3d& initValue);

	//! Resets the bounding box.
	void reset(const vector3d& initValue);
	
	//! returns center of the bounding box
	vector3d getCenter() const;

	//! returns extent of the box
	vector3d getExtent() const;

	//! stores all 8 edges of the box into a array
	//! \param edges: Pointer to array of 8 edges
	void getEdges(vector3d* edges) const;

	//! returns if the box is empty, which means that there is
	//! no space within the min and the max edge.
	bool isEmpty() const;

	//! repairs the box, if for example MinEdge and MaxEdge are swapped.
	void repair();

	vector3d MinEdge;
	vector3d MaxEdge;
};

} // end namespace core

#endif
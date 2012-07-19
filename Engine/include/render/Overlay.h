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

#ifndef _OVERLAY_H_
#define _OVERLAY_H_

#include <core/Config.h>
#include <engine/Object.h>
#include <render/Renderable.h>

namespace render
{

enum MetricsMode
{
	MM_RELATIVE,				// 'left', 'top', 'height' and 'width' are parametrics from 0.0 to 1.0
	MM_PIXELS,					// Positions & sizes are in absolute pixels
	MM_RELATIVE_ASPECT_ADJUSTED	// Positions & sizes are in virtual pixels
};

//! Enum describing where '0' is in relation to the parent in the horizontal dimension.
enum HorizontalAlignment
{
	HA_LEFT,
	HA_CENTER,
	HA_RIGHT
};

//! Enum describing where '0' is in relation to the parent in the vertical dimension.
enum VerticalAlignment
{
	VA_TOP,
	VA_CENTER,
	VA_BOTTOM
};


class Material;
class VertexData;
class IndexData;

//! Represents a layer which is rendered on top of the 'normal' scene contents.
class ENGINE_PUBLIC_EXPORT Overlay: public Renderable
{
public:

	Overlay();
	Overlay(const std::string& name);

	virtual ~Overlay();

	void setVisible(bool enabled);
	bool isVisible();

	//!	Sets the position.
	void setPosition(float x, float y);

	//!	Sets the dimension.
	void setDimension(float width, float height);

	//! Gets one of the relative dimensions of the overlay, a value between 0.0 and 1.0.
	float getWidth();
	//! Gets one of the relative dimensions of the overlay, a value between 0.0 and 1.0.
	float getHeight();

	//! Sets the MetricsMode for this overlay.
	virtual void setMetricsMode(MetricsMode mm);
	//!  Gets the MetricsMode of this overlay.
	MetricsMode getMetricsMode() const;

	//! Gets the material which this mesh uses.
	virtual Material* getMaterial() const;

protected:

	// Incremented count for next Index
	static unsigned int msNextGeneratedOverlayIndex;

	bool mIsVisible;

	Material* mMaterial;

	float mWidth;
	float mHeight;

	MetricsMode mMetricsMode;

	float mU1, mV1, mU2, mV2;

	void updateImpl(float elapsedTime);

	bool mGeometryInitialized;

	//! Flag indicating if the vertex positions need recalculating.
	bool mGeomPositionsOutOfDate;
	//! Flag indicating if the vertex uvs need recalculating.
	bool mGeomUVsOutOfDate;

	//! Internal method for updating the positions of the element.
	virtual void updatePositionBinding() = 0;
	//! Internal method for updating the UVs of the element.
	virtual void updateTextureBinding() = 0;

	bool mModifiedWorldTransform;

	void updateWorldTransform();
};

} //namespace render

#endif
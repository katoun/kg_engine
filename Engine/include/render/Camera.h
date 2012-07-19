/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2012 Catalin Alexandru Nastase

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

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <core/Config.h>
#include <core/Matrix4.h>
#include <core/Vector3d.h>
#include <game/Component.h>

namespace render
{

class Frustum;
enum ProjectionType;
enum SceneDetailLevel;

//!	A viewpoint from which the scene will be rendered.

//!	K_Game renders scenes from a camera viewpoint into a buffer of
//!	some sort, normally a window or a texture (a subclass of
//!	RenderTarget). K_Game cameras support both perspective projection (the default,
//!	meaning objects get smaller the further away they are) and
//!	orthographic projection (blueprint-style, no decrease in size
//!	with distance). In addition, more than one camera can point at 
//! a single render target if required, each rendering to a subset of the target, 
//! allowing split screen and picture-in-picture views.

//!	Cameras maintain their own aspect ratios, field of view, and frustum,
//!	and project co-ordinates into a space measured from -1 to 1 in x and y,
//!	and 0 to 1 in z. At render time, the camera will be rendering to a
//!	Viewport which will translate these parametric co-ordinates into real screen
//!	co-ordinates. Obviously it is advisable that the viewport has the same
//!	aspect ratio as the camera to avoid distortion (unless you want it!).
class ENGINE_PUBLIC_EXPORT Camera: public game::Component
{
public:

	Camera();
	~Camera();

	//! Sets the type of projection to use (orthographic or perspective). Default is perspective.
	void setProjectionType(ProjectionType pt);
	//! Retrieves info on the type of projection used (orthographic or perspective).
	ProjectionType getProjectionType();

	//! Sets the level of rendering detail required from this camera.

	//! Each camera is set to render at full detail by default, that is
	//! with full texturing, lighting etc. This method lets you change
	//! that behavior, allowing you to make the camera just render a
	//! wireframe view, for example.
	void setDetailLevel(SceneDetailLevel sd);
	//! Retrieves the level of detail that the camera will render.
	SceneDetailLevel getDetailLevel();

	bool getVisibleFrustum();
	void setVisibleFrustum(bool visible);
	
	bool getFixedUp();
	const core::vector3d& getFixedUpAxis();

	void setFixedUpAxis(bool useFixed, const core::vector3d& fixedAxis = core::vector3d::UNIT_Y);
	
	//! Sets the Y-dimension Field Of View (FOV) of the camera.
	//! Field Of View (FOV) is the angle made between the camera's position, and the left & right edges
	//! of the 'screen' onto which the scene is projected. High values (90+) result in a wide-angle,
	//! fish-eye kind of view, low values (30-) in a stretched, telescopic kind of view. Typical values
	//! are between 45 and 60.<BR>
	//! This value represents the HORIZONTAL field-of-view. The vertical
	//! field of view is calculated from this depending on the dimensions
	//! of the viewport (they will only be the same if the viewport is square).
	void setFOV(float fov);
	//! Retrieves the cameras Y-dimension Field Of View (FOV).
	float getFOV();
	
	//! Sets the position of the near clipping plane.
	//! The position of the near clipping plane is the distance from the
	//! cameras position to the screen on which the world is projected. The
	//! near plane distance, combined with the field-of-view and the
	//! aspect ratio, determines the size of the viewport through which
	//! the world is viewed (in world co-ordinates). Note that this world
	//! viewport is different to a screen viewport, which has it's dimensions
	//! expressed in pixels. The cameras viewport should have the same aspect
	//! ratio as the screen viewport it renders into to avoid distortion.
	//! \param nearDist: The distance to the near clipping plane from the camera in world coordinates.
	void setNearClipDistance(float nearDist);

	//! Sets the position of the near clipping plane.
	float getNearClipDistance();

	//! Sets the distance to the far clipping plane.

	//! The view frustum is a pyramid created from the camera position and the edges of the viewport.
	//! This frustum does not extend to infinity - it is cropped near to the camera and there is a far
	//! plane beyond which nothing is displayed. This method sets the distance for the far plane. Different
	//! applications need different values: e.g. a flight sim needs a much further far clipping plane than
	//! a first-person shooter. An important point here is that the larger the gap between near and far
	//! clipping planes, the lower the accuracy of the Z-buffer used to depth-cue pixels. This is because the
	//! Z-range is limited to the size of the Z buffer (16 or 32-bit) and the max values must be spread over
	//! the gap between near and far clip planes. The bigger the range, the more the Z values will
	//! be approximated which can cause artifacts when lots of objects are close together in the Z-plane. So
	//! make sure you clip as close to the camera as you can - don't set a huge value for the sake of it.
	//! \param farDist: The distance to the far clipping plane from the camera in world coordinates.
	void setFarClipDistance(float farDist);

	//! Retrieves the distance from the camera to the far clipping plane.
	float getFarClipDistance();

	//! Sets the aspect ratio for the camera viewport.
	//! The ratio between the x and y dimensions of the rectangular
	//! area visible through the camera is known as aspect ratio:
	//! aspect = width / height .
	//! The default for most fullscreen windows is 1.3333 - this is
	//! also assumed by Ogre unless you use this method to state otherwise.
	void setAspectRatio(float ratio);

	//! Retrieves the current aspect ratio.
	float getAspectRatio();

	//! Gets the projection matrix for this frustum adjusted for the current rendersystem specifics.
	const core::matrix4& getProjectionMatrixRS();

	//! Gets the projection matrix for this camera.
	//! The projection matrix which conforms to standard right-handed rules and
	//! uses depth range [-1,+1].
	const core::matrix4& getProjectionMatrix();

	//! Gets the view matrix for this camera.
	const core::matrix4& getViewMatrix();

	//! get the frustum of this camera.
	Frustum* getFrustum() const;

protected:

	void updateImpl(float elapsedTime);
	void onMessageImpl(unsigned int messageID);

	void updateProjection();
	void updateView();
	void updateFrustum();

	// Whether to use a fixed Up axis.
	bool mFixedUp;
	// Fixed Up axis
	core::vector3d mFixedUpAxis;

	float mFOV;				// Camera y-direction field-of-view (default 45)
	float mAspect;			// x/y viewport ratio - default 1.3333
	float mNearDist;		// Near clip distance - default 1
	float mFarDist;			// Far clip distance - default 1000	

	Frustum* mFrustum;

	ProjectionType mProjType;		// Orthographic or perspective?
	SceneDetailLevel mSceneDetail;	// Rendering type

	bool mVisibleFrustum;

	core::matrix4 mProjMatrixRS;	// Pre-calced projection matrix for the specific render system
	core::matrix4 mProjMatrix;		// Pre-calced projection matrix
	core::matrix4 mViewMatrix;		// Pre-calced view matrix	
	
	bool mProjectionNeedsUpdate;	// Projection needs an update?
	bool mViewNeedsUpdate;			// View needs an update?
	bool mFrustumNeedsUpdate;		// Frustum needs an update ?
};

} //namespace render

#endif
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

#include <core/Log.h>
#include <core/Math.h>
#include <core/Sphere3d.h>
#include <core/Aabox3d.h>
#include <core/LogDefines.h>
#include <render/Light.h>
#include <render/Viewport.h>
#include <render/Shader.h>
#include <render/Renderable.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>
#include <game/GameObject.h>
#include <game/ComponentDefines.h>
#include <game/Transform.h>
#include <GLRenderDriver.h>
#include <GLTexture.h>
#include <GLShader.h>
#include <GLVertexBuffer.h>
#include <GLIndexBuffer.h>

#if ENGINE_PLATFORM == PLATFORM_WIN32
#include <win32/Win32Window.h>
#endif

#define VBO_BUFFER_OFFSET(i) ((char *)NULL + (i))

template<> render::GLRenderDriver& core::Singleton<render::GLRenderDriver>::ms_Singleton = render::GLRenderDriver();

namespace render
{

GLRenderDriver::GLRenderDriver(): RenderDriver("OpenGL RenderDriver")
{
	mCgContext = NULL;
}

GLRenderDriver::~GLRenderDriver() {}

RenderWindow* GLRenderDriver::createRenderWindow(signed int width, signed int height, signed int colorDepth, bool fullScreen, signed int left, signed int top, bool depthBuffer, void* windowId)
{
	// Create the window
	RenderWindow* pRenderWindow = NULL;

#if ENGINE_PLATFORM == PLATFORM_WIN32
	pRenderWindow = new Win32Window();
#endif

	if (pRenderWindow != NULL)
	{
		pRenderWindow->create(width, height, colorDepth, fullScreen, left, top, depthBuffer, windowId);
	}

	return pRenderWindow;
}

VertexBuffer* GLRenderDriver::createVertexBuffer(unsigned int vertexSize, unsigned int numVertices, resource::BufferUsage usage)
{
	VertexBuffer* buf = new GLVertexBuffer(vertexSize, numVertices, usage);

	return buf;
}

void GLRenderDriver::removeVertexBuffer(VertexBuffer* buf)
{
	GLVertexBuffer* GLbuf = static_cast<GLVertexBuffer*>(buf);

	assert(GLbuf);
	delete GLbuf;
}

IndexBuffer* GLRenderDriver::createIndexBuffer(IndexType idxType, unsigned int numIndexes, resource::BufferUsage usage)
{
	IndexBuffer* buf = new GLIndexBuffer(idxType, numIndexes, usage);

	return buf;
}

void GLRenderDriver::removeIndexBuffer(IndexBuffer* buf)
{
	GLIndexBuffer* GLbuf = static_cast<GLIndexBuffer*>(buf);

	assert(GLbuf);
	delete GLbuf;
}

void GLRenderDriver::beginFrame(Viewport* vp)
{
	// Clear the viewport if required
	if (vp->getClearEveryFrame())
	{
		// Activate the viewport clipping
		glEnable(GL_SCISSOR_TEST);

		Color col = vp->getBackgroundColor();

		glClearColor(col.R, col.G, col.B, col.A);

		glClearDepth(1.0f);									// Depth Buffer Setup
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	}
}

void GLRenderDriver::render(Renderable* renderable)
{
	if (renderable == NULL)
		return;

	if (renderable->getVertexData() == NULL || renderable->getIndexData() == NULL)
		return;
	
	void* pBufferData = NULL;

	const std::list<VertexElement*>& elems = renderable->getVertexData()->vertexDeclaration->getElements();
	std::list<VertexElement*>::const_iterator i;
	for (i = elems.begin(); i != elems.end(); ++i)
	{
		VertexElement* elem = (*i);

		VertexBuffer* vertexBuffer = renderable->getVertexData()->vertexBufferBinding->getBuffer(elem->getSource());

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, ((const GLVertexBuffer*)(vertexBuffer))->getGLBufferId());
		pBufferData = VBO_BUFFER_OFFSET(elem->getOffset());

		switch(elem->getSemantic())
		{
		case VES_POSITION:
			glVertexPointer(VertexElement::getTypeCount(elem->getType()), 
				getGLType(elem->getType()), 
				(GLsizei)(vertexBuffer->getVertexSize()), 
				pBufferData);
			glEnableClientState(GL_VERTEX_ARRAY);
			break;
		case VES_NORMAL:
			glNormalPointer(
				getGLType(elem->getType()), 
				(GLsizei)(vertexBuffer->getVertexSize()), 
				pBufferData);
			glEnableClientState(GL_NORMAL_ARRAY);
			break;
		case VES_DIFFUSE:
			glColorPointer(4, 
				getGLType(elem->getType()), 
				(GLsizei)(vertexBuffer->getVertexSize()), 
				pBufferData);
			glEnableClientState(GL_COLOR_ARRAY);
			break;
		//case VES_SPECULAR:
		//	glSecondaryColorPointerEXT(4, 
		//		getGLType(elem->getType()), 
		//		(GLsizei)(vertexBuffer->getVertexSize()), 
		//		pBufferData);
		//	glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
		//	break;
		case VES_TEXTURE_COORDINATES:
			for (unsigned int j = 0; j < mDisabledTexUnitsFrom; ++j)
			{
				if (mTextureCoordIndex[j] == elem->getIndex())
				{
					glClientActiveTextureARB(GL_TEXTURE0 + j);
					if (glIsEnabled(GL_TEXTURE_2D))
					{
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(VertexElement::getTypeCount(elem->getType()), 
							getGLType(elem->getType()),
							(GLsizei)(vertexBuffer->getVertexSize()), 
							pBufferData);
					}
				}
			}
			break;
		default:
			break;
		}
	}

	glClientActiveTextureARB(GL_TEXTURE0);

	// Find the correct type to render
	GLint primType = GL_TRIANGLES;
	switch (renderable->getRenderOperationType())
	{
	case render::ROT_POINT_LIST:
		primType = GL_POINTS;
		break;
	case render::ROT_LINE_LIST:
		primType = GL_LINES;
		break;
	case render::ROT_LINE_STRIP:
		primType = GL_LINE_STRIP;
		break;
	case render::ROT_TRIANGLE_LIST:
		primType = GL_TRIANGLES;
		break;
	case render::ROT_TRIANGLE_STRIP:
		primType = GL_TRIANGLE_STRIP;
		break;
	case render::ROT_TRIANGLE_FAN:
		primType = GL_TRIANGLE_FAN;
		break;
	}

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, ((GLIndexBuffer*)(renderable->getIndexData()->indexBuffer))->getGLBufferId());
	pBufferData = VBO_BUFFER_OFFSET(0);
	
	GLenum indexType = (renderable->getIndexData()->indexBuffer->getType() == IT_16BIT) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
	
	glDrawElements(primType, renderable->getIndexData()->indexCount, indexType, pBufferData);

	glDisableClientState(GL_VERTEX_ARRAY);
	for (unsigned int j = 0; j < ENGINE_MAX_TEXTURE_COORD_SETS; ++j)
	{
		glClientActiveTextureARB(GL_TEXTURE0 + j);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	glClientActiveTextureARB(GL_TEXTURE0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	//glSecondaryColor3fEXT(0.0f, 0.0f, 0.0f);
}

void GLRenderDriver::renderGridPlane(unsigned int zfar)
{
	float planeLimit = (float)(zfar);
	float planeCoord = (float)(zfar / ENGINE_UNIT_M);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	// Top Face
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-planeLimit,  0.0f, -planeLimit);
	glTexCoord2f(0.0f, planeCoord);				glVertex3f(-planeLimit,  0.0f,  planeLimit);
	glTexCoord2f(planeCoord, planeCoord);			glVertex3f( planeLimit,  0.0f,  planeLimit);
	glTexCoord2f(planeCoord, 0.0f);	glVertex3f( planeLimit,  0.0f, -planeLimit);
	glEnd();
}

void GLRenderDriver::renderWorldAxes()
{
	glLineWidth(3.0f);									// Set The Line Width

	glBegin(GL_LINES);
	// X axis
	glColor3f(1.0f, 0.0f, 0.0f);						// Red
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(50.0f, 0.0f, 0.0f);

	// Y axis
	glColor3f(0.0f, 1.0f, 0.0f);						// Green
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 50.0f, 0.0f);

	// Z axis
	glColor3f(0.0f, 0.0f, 1.0f);						// Blue
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 50.0f);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	// X axis
	glColor3f(1.0f, 0.0f, 0.0f);						// Red
	glVertex3f(50.0f, 0.0f, 0.0f);
	glVertex3f(44.0f, 2.0f, 0.0f);
	glVertex3f(44.0f, 0.0f, 2.0f);
	glVertex3f(44.0f, -2.0f, 0.0f);
	glVertex3f(44.0f, 0.0f, -2.0f);
	glVertex3f(44.0f, 2.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	// Y axis
	glColor3f(0.0f, 1.0f, 0.0f);						// Green
	glVertex3f(0.0f, 50.0f, 0.0f);
	glVertex3f(2.0f, 44.0f, 0.0f);
	glVertex3f(0.0f, 44.0f, 2.0f);
	glVertex3f(-2.0f, 44.0f, 0.0f);
	glVertex3f(0.0f, 44.0f, -2.0f);
	glVertex3f(2.0f, 44.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	// Z axis
	glColor3f(0.0f, 0.0f, 1.0f);						// Blue
	glVertex3f(0.0f, 0.0f, 50.0f);
	glVertex3f(2.0f, 0.0f, 44.0f);
	glVertex3f(0.0f, 2.0f, 44.0f);
	glVertex3f(-2.0f, 0.0f, 44.0f);
	glVertex3f(0.0f, -2.0f, 44.0f);
	glVertex3f(2.0f, 0.0f, 44.0f);
	glEnd();
}

void GLRenderDriver::renderAxes(const core::vector3d& position, const core::quaternion& orientation)
{
	core::vector3d axisX = orientation * core::vector3d::UNIT_X;
	core::vector3d axisY = orientation * core::vector3d::UNIT_Y;
	core::vector3d axisZ = orientation * core::vector3d::UNIT_Z;
	axisX.normalize();
	axisY.normalize();
	axisZ.normalize();
	axisX = position + axisX * 2 * ENGINE_UNIT_M;
	axisY = position + axisY * 2 * ENGINE_UNIT_M;
	axisZ = position + axisZ * 2 * ENGINE_UNIT_M;

	glLineWidth(3.0f);									// Set The Line Width

	glBegin(GL_LINES);
	// X axis
	glColor3f(1.0f, 0.0f, 0.0f);						// Red
	glVertex3f(position.X, position.Y, position.Z);
	glVertex3f(axisX.X, axisX.Y, axisX.Z);
	// Y axis
	glColor3f(0.0f, 1.0f, 0.0f);						// Green
	glVertex3f(position.X, position.Y, position.Z);
	glVertex3f(axisY.X, axisY.Y, axisY.Z);
	// Z axis
	glColor3f(0.0f, 0.0f, 1.0f);						// Blue
	glVertex3f(position.X, position.Y, position.Z);
	glVertex3f(axisZ.X, axisZ.Y, axisZ.Z);
	glEnd();
}

void GLRenderDriver::renderBoundingSphere(const core::sphere3d& sphere)
{
	int segments = 32;

	float theta = 2 * core::PI / (float)segments;
	float tangetial_factor = core::tan(theta);//calculate the tangential factor
	float radial_factor = core::cos(theta);//calculate the radial factor
	
	glLineWidth(1.0f);									// Set The Line Width

	glColor3f(0.3f, 0.7f, 0.3f);
	
	float x = sphere.Radius;//we start at angle = 0 
	float y = 0;
	float z = 0;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < segments; i++)
	{
		glVertex3f(x + sphere.Center.X, y + sphere.Center.Y, sphere.Center.Z);

		//calculate the tangential vector
		//remember, the radial vector is (x, y)
		//to get the tangential vector we flip those coordinates and negate one of them

		float tx = -y;
		float ty = x;

		//add the tangential vector

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		//correct using the radial factor

		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();

	x = sphere.Radius;//we start at angle = 0 
	z = 0; 
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < segments; i++)
	{
		glVertex3f(x + sphere.Center.X, sphere.Center.Y, z + sphere.Center.Z);

		float tx = -z;
		float tz = x;

		x += tx * tangetial_factor;
		z += tz * tangetial_factor;

		x *= radial_factor;
		z *= radial_factor;
	}
	glEnd();

	y = sphere.Radius;//we start at angle = 0 
	z = 0; 
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < segments; i++)
	{
		glVertex3f(sphere.Center.X, y + sphere.Center.Y, z + sphere.Center.Z);

		float ty = -z;
		float tz = y;

		y += ty * tangetial_factor;
		z += tz * tangetial_factor;

		y *= radial_factor;
		z *= radial_factor;
	}
	glEnd();
}

void GLRenderDriver::renderBoundingBox(const core::aabox3d& box)
{
	glColor3f(0.3f, 0.7f, 0.3f);
	glBegin(GL_LINES);
	//Bottom plane
	glVertex3f(box.MinEdge.X, box.MinEdge.Y, box.MinEdge.Z);
	glVertex3f(box.MaxEdge.X, box.MinEdge.Y, box.MinEdge.Z);

	glVertex3f(box.MaxEdge.X, box.MinEdge.Y, box.MinEdge.Z);
	glVertex3f(box.MaxEdge.X, box.MinEdge.Y, box.MaxEdge.Z);

	glVertex3f(box.MinEdge.X, box.MinEdge.Y, box.MaxEdge.Z);
	glVertex3f(box.MaxEdge.X, box.MinEdge.Y, box.MaxEdge.Z);

	glVertex3f(box.MinEdge.X, box.MinEdge.Y, box.MinEdge.Z);
	glVertex3f(box.MinEdge.X, box.MinEdge.Y, box.MaxEdge.Z);

	//Upper plane
	glVertex3f(box.MinEdge.X, box.MaxEdge.Y, box.MinEdge.Z);
	glVertex3f(box.MaxEdge.X, box.MaxEdge.Y, box.MinEdge.Z);

	glVertex3f(box.MaxEdge.X, box.MaxEdge.Y, box.MinEdge.Z);
	glVertex3f(box.MaxEdge.X, box.MaxEdge.Y, box.MaxEdge.Z);

	glVertex3f(box.MinEdge.X, box.MaxEdge.Y, box.MaxEdge.Z);
	glVertex3f(box.MaxEdge.X, box.MaxEdge.Y, box.MaxEdge.Z);

	glVertex3f(box.MinEdge.X, box.MaxEdge.Y, box.MinEdge.Z);
	glVertex3f(box.MinEdge.X, box.MaxEdge.Y, box.MaxEdge.Z);

	//Left plane
	glVertex3f(box.MinEdge.X, box.MinEdge.Y, box.MinEdge.Z);
	glVertex3f(box.MinEdge.X, box.MaxEdge.Y, box.MinEdge.Z);

	glVertex3f(box.MinEdge.X, box.MinEdge.Y, box.MaxEdge.Z);
	glVertex3f(box.MinEdge.X, box.MaxEdge.Y, box.MaxEdge.Z);

	//Right plane
	glVertex3f(box.MaxEdge.X, box.MinEdge.Y, box.MinEdge.Z);
	glVertex3f(box.MaxEdge.X, box.MaxEdge.Y, box.MinEdge.Z);

	glVertex3f(box.MaxEdge.X, box.MinEdge.Y, box.MaxEdge.Z);
	glVertex3f(box.MaxEdge.X, box.MaxEdge.Y, box.MaxEdge.Z);
	glEnd();
}

void GLRenderDriver::renderFrustumVolume(const core::vector3d* corners)
{
	glLineWidth(1.0f);									// Set The Line Width

	glColor3f(1.0f, 1.0f, 0.5f);
	glBegin(GL_LINES);

	//Front plane
	glVertex3f(corners[0].X, corners[0].Y, corners[0].Z);
	glVertex3f(corners[3].X, corners[3].Y, corners[3].Z);

	glVertex3f(corners[1].X, corners[1].Y, corners[1].Z);
	glVertex3f(corners[2].X, corners[2].Y, corners[2].Z);

	glVertex3f(corners[0].X, corners[0].Y, corners[0].Z);
	glVertex3f(corners[1].X, corners[1].Y, corners[1].Z);

	glVertex3f(corners[3].X, corners[3].Y, corners[3].Z);
	glVertex3f(corners[2].X, corners[2].Y, corners[2].Z);

	//Back plane
	glVertex3f(corners[4].X, corners[4].Y, corners[4].Z);
	glVertex3f(corners[7].X, corners[7].Y, corners[7].Z);

	glVertex3f(corners[5].X, corners[5].Y, corners[5].Z);
	glVertex3f(corners[6].X, corners[6].Y, corners[6].Z);

	glVertex3f(corners[4].X, corners[4].Y, corners[4].Z);
	glVertex3f(corners[5].X, corners[5].Y, corners[5].Z);

	glVertex3f(corners[7].X, corners[7].Y, corners[7].Z);
	glVertex3f(corners[6].X, corners[6].Y, corners[6].Z);

	//Left plane
	glVertex3f(corners[0].X, corners[0].Y, corners[0].Z);
	glVertex3f(corners[4].X, corners[4].Y, corners[4].Z);

	glVertex3f(corners[1].X, corners[1].Y, corners[1].Z);
	glVertex3f(corners[5].X, corners[5].Y, corners[5].Z);

	//Right plane
	glVertex3f(corners[3].X, corners[3].Y, corners[3].Z);
	glVertex3f(corners[7].X, corners[7].Y, corners[7].Z);

	glVertex3f(corners[2].X, corners[2].Y, corners[2].Z);
	glVertex3f(corners[6].X, corners[6].Y, corners[6].Z);

	glEnd();

	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	// enables some blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// draws this volume
	glColor3f(0.08f, 0.20f, 0.08f);
	glBegin(GL_QUADS);
	//Front plane
	glVertex3f(corners[0].X, corners[0].Y, corners[0].Z);
	glVertex3f(corners[1].X, corners[1].Y, corners[1].Z);
	glVertex3f(corners[2].X, corners[2].Y, corners[2].Z);
	glVertex3f(corners[3].X, corners[3].Y, corners[3].Z);

	//Back plane
	glVertex3f(corners[7].X, corners[7].Y, corners[7].Z);
	glVertex3f(corners[6].X, corners[6].Y, corners[6].Z);
	glVertex3f(corners[5].X, corners[5].Y, corners[5].Z);
	glVertex3f(corners[4].X, corners[4].Y, corners[4].Z);

	//Left plane
	glVertex3f(corners[4].X, corners[4].Y, corners[4].Z);
	glVertex3f(corners[5].X, corners[5].Y, corners[5].Z);
	glVertex3f(corners[1].X, corners[1].Y, corners[1].Z);
	glVertex3f(corners[0].X, corners[0].Y, corners[0].Z);	

	//Right plane
	glVertex3f(corners[3].X, corners[3].Y, corners[3].Z);
	glVertex3f(corners[2].X, corners[2].Y, corners[2].Z);
	glVertex3f(corners[6].X, corners[6].Y, corners[6].Z);
	glVertex3f(corners[7].X, corners[7].Y, corners[7].Z);

	//Top plane	
	glVertex3f(corners[0].X, corners[0].Y, corners[0].Z);
	glVertex3f(corners[3].X, corners[3].Y, corners[3].Z);
	glVertex3f(corners[7].X, corners[7].Y, corners[7].Z);
	glVertex3f(corners[4].X, corners[4].Y, corners[4].Z);
	
	//Bottom plane
	glVertex3f(corners[2].X, corners[2].Y, corners[2].Z);
	glVertex3f(corners[1].X, corners[1].Y, corners[1].Z);
	glVertex3f(corners[5].X, corners[5].Y, corners[5].Z);
	glVertex3f(corners[6].X, corners[6].Y, corners[6].Z);
	
	glEnd();
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDisable(GL_BLEND);
}

void GLRenderDriver::endFrame()
{
	// Deactivate the viewport clipping
	glDisable(GL_SCISSOR_TEST);
}

void GLRenderDriver::setViewport(Viewport* vp)
{
	// XXX Rendering target stuff?
	GLsizei x, y, w, h;

	RenderTarget* target;
	target = vp->getTarget();

	// Calculate the "lower-left" corner of the viewport
	w = vp->getActualWidth();
	h = vp->getActualHeight();
	x = vp->getActualLeft();
	y = target->getHeight() - vp->getActualTop() - h;

	glViewport(x, y, w, h);

	// Configure the viewport clipping
	glScissor(x, y, w, h);
}

void GLRenderDriver::setWorldMatrix(const core::matrix4& m)
{
	mWorldMatrix = m;

	GLfloat mat[16];
	makeGLMatrix(mat, mViewMatrix * mWorldMatrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mat);
}

void GLRenderDriver::setViewMatrix(const core::matrix4& m)
{
	mViewMatrix = m;

	GLfloat mat[16];
	makeGLMatrix(mat, mViewMatrix * mWorldMatrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mat);
}

void GLRenderDriver::setProjectionMatrix(const core::matrix4& m)
{	
	GLfloat mat[16];
	
	mProjMatrix = m;
	
	makeGLMatrix(mat, mProjMatrix);
	
	glMatrixMode(GL_PROJECTION);// Select The Projection Matrix
	glLoadMatrixf(mat);	

	glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
}

void GLRenderDriver::setShadingType(ShadeOptions so)
{	
	switch(so)
	{
	case SO_FLAT:
		glShadeModel(GL_FLAT);		// Enable Flat Shading
		break;
	default:
		glShadeModel(GL_SMOOTH);	// Enable Smooth Shading
		break;
	}						
}

void GLRenderDriver::setSurfaceParams(const Color& ambient, const Color& diffuse, 
									  const Color& specular, const Color& emissive, float shininess)
{
	GLfloat f4val[4] = {diffuse.R, diffuse.G, diffuse.B, diffuse.A};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, f4val);
	f4val[0] = ambient.R;
	f4val[1] = ambient.G;
	f4val[2] = ambient.B;
	f4val[3] = ambient.A;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, f4val);
	f4val[0] = specular.R;
	f4val[1] = specular.G;
	f4val[2] = specular.B;
	f4val[3] = specular.A;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, f4val);
	f4val[0] = emissive.R;
	f4val[1] = emissive.G;
	f4val[2] = emissive.B;
	f4val[3] = emissive.A;
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, f4val);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void GLRenderDriver::setSceneBlending(SceneBlendFactor sourceFactor, SceneBlendFactor destFactor)
{
	GLint sourceBlend = getGLBlendMode(sourceFactor);
	GLint destBlend = getGLBlendMode(destFactor);

	if (sourceFactor == SBF_ONE && destFactor == SBF_ZERO)
	{
		glDisable(GL_BLEND);
	}
	else
	{
		glEnable(GL_BLEND);
		glBlendFunc(sourceBlend, destBlend);
	}
}

void GLRenderDriver::setTexture(bool enabled, unsigned int unit, Texture* tex)
{
	GLTexture* glTex = static_cast<GLTexture*>(tex);

	glActiveTextureARB(GL_TEXTURE0 + unit);
	if (enabled && glTex)
	{
		glEnable(GL_TEXTURE_2D);										// Enable Texture Mapping	
		glBindTexture(GL_TEXTURE_2D, glTex->getGLID());
	}
	else
	{
		glDisable(GL_TEXTURE_2D);										// Disable Texture Mapping
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	glActiveTextureARB(GL_TEXTURE0);
}

void GLRenderDriver::setTextureBlendMode(unsigned int unit, const LayerBlendMode& bm)
{
	GLenum src1op, src2op, cmd;
	GLfloat cv1[4], cv2[4];

	if (bm.blendType == LBT_COLOR)
	{
		cv1[0] = bm.colorArg1.R;
		cv1[1] = bm.colorArg1.G;
		cv1[2] = bm.colorArg1.B;
		cv1[3] = bm.colorArg1.A;
		mManualBlendColors[unit][0] = bm.colorArg1;


		cv2[0] = bm.colorArg2.R;
		cv2[1] = bm.colorArg2.G;
		cv2[2] = bm.colorArg2.B;
		cv2[3] = bm.colorArg2.A;
		mManualBlendColors[unit][1] = bm.colorArg2;
	}

	if (bm.blendType == LBT_ALPHA)
	{
		cv1[0] = mManualBlendColors[unit][0].R;
		cv1[1] = mManualBlendColors[unit][0].G;
		cv1[2] = mManualBlendColors[unit][0].B;
		cv1[3] = bm.alphaArg1;

		cv2[0] = mManualBlendColors[unit][1].R;
		cv2[1] = mManualBlendColors[unit][1].G;
		cv2[2] = mManualBlendColors[unit][1].B;
		cv2[3] = bm.alphaArg2;
	}

	switch (bm.source1)
	{
	case LBS_CURRENT:
		src1op = GL_PREVIOUS;
		break;
	case LBS_TEXTURE:
		src1op = GL_TEXTURE;
		break;
	case LBS_MANUAL:
		src1op = GL_CONSTANT;
		break;
	case LBS_DIFFUSE:
		src1op = GL_PRIMARY_COLOR;
		break;
	case LBS_SPECULAR:
		src1op = GL_PRIMARY_COLOR;
		break;
	default:
		src1op = 0;
	}

	switch (bm.source2)
	{
	case LBS_CURRENT:
		src2op = GL_PREVIOUS;
		break;
	case LBS_TEXTURE:
		src2op = GL_TEXTURE;
		break;
	case LBS_MANUAL:
		src2op = GL_CONSTANT;
		break;
	case LBS_DIFFUSE:
		src2op = GL_PRIMARY_COLOR;
		break;
	case LBS_SPECULAR:
		src2op = GL_PRIMARY_COLOR;
		break;
	default:
		src2op = 0;
	}

	switch (bm.operation)
	{
	case LBX_SOURCE1:
		cmd = GL_REPLACE;
		break;
	case LBX_SOURCE2:
		cmd = GL_REPLACE;
		break;
	case LBX_MODULATE:
		cmd = GL_MODULATE;
		break;
	case LBX_MODULATE_X2:
		cmd = GL_MODULATE;
		break;
	case LBX_MODULATE_X4:
		cmd = GL_MODULATE;
		break;
	case LBX_ADD:
		cmd = GL_ADD;
		break;
	case LBX_ADD_SIGNED:
		cmd = GL_ADD_SIGNED;
		break;
	case LBX_ADD_SMOOTH:
		cmd = GL_INTERPOLATE;
		break;
	case LBX_SUBTRACT:
		cmd = GL_SUBTRACT;
		break;
	case LBX_BLEND_DIFFUSE_COLOR:
		cmd = GL_INTERPOLATE;
		break; 
	case LBX_BLEND_DIFFUSE_ALPHA:
		cmd = GL_INTERPOLATE;
		break;
	case LBX_BLEND_TEXTURE_ALPHA:
		cmd = GL_INTERPOLATE;
		break;
	case LBX_BLEND_CURRENT_ALPHA:
		cmd = GL_INTERPOLATE;
		break;
	case LBX_BLEND_MANUAL:
		cmd = GL_INTERPOLATE;
		break;
	case LBX_DOTPRODUCT:
		cmd = GL_DOT3_RGB;
	break;
		default:
		cmd = 0;
	}

	glActiveTextureARB(GL_TEXTURE0 + unit);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

	if (bm.blendType == LBT_COLOR)
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, cmd);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, src1op);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, src2op);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_CONSTANT);
	}
	else
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, cmd);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, src1op);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, src2op);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA, GL_CONSTANT);
	}

	float blendValue[4] = {0, 0, 0, bm.factor};
	switch (bm.operation)
	{
	case LBX_BLEND_DIFFUSE_COLOR:
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PRIMARY_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA, GL_PRIMARY_COLOR);
		break;
	case LBX_BLEND_DIFFUSE_ALPHA:
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PRIMARY_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA, GL_PRIMARY_COLOR);
		break;
	case LBX_BLEND_TEXTURE_ALPHA:
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA, GL_TEXTURE);
		break;
	case LBX_BLEND_CURRENT_ALPHA:
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PREVIOUS);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA, GL_PREVIOUS);
		break;
	case LBX_BLEND_MANUAL:
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, blendValue);
		break;
	default:
		break;
	}

	switch (bm.operation)
	{
	case LBX_MODULATE_X2:
		glTexEnvi(GL_TEXTURE_ENV, bm.blendType == LBT_COLOR ? GL_RGB_SCALE : GL_ALPHA_SCALE, 2);
		break;
	case LBX_MODULATE_X4:
		glTexEnvi(GL_TEXTURE_ENV, bm.blendType == LBT_COLOR ? GL_RGB_SCALE : GL_ALPHA_SCALE, 4);
	break;
	default:
		glTexEnvi(GL_TEXTURE_ENV, bm.blendType == LBT_COLOR ? GL_RGB_SCALE : GL_ALPHA_SCALE, 1);
		break;
	}

	if (bm.blendType == LBT_COLOR)
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
		if (bm.operation == LBX_BLEND_DIFFUSE_COLOR)
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_COLOR);
		} 
		else
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);
		}
	}

	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, GL_SRC_ALPHA);
	if (bm.source1 == LBS_MANUAL)
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, cv1);
	if (bm.source2 == LBS_MANUAL)
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, cv2);

	glActiveTextureARB(GL_TEXTURE0);
}

void GLRenderDriver::setLightingEnabled(bool enabled)
{
	if (enabled)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

void GLRenderDriver::setAmbientLight(float red, float green, float blue, float alpha)
{
	GLfloat f4vals[4] = {red, green, blue, alpha};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, f4vals);
}

void GLRenderDriver::setLights(const std::vector<Light*>& lights)
{
	std::vector<Light*>::const_iterator i;
	unsigned int j;
	for (i = lights.begin(), j=0; i != lights.end() && j < ENGINE_MAX_SIMULTANEOUS_LIGHTS; i++, j++)
	{
		mLights[j] = (*i);
		setGLLight(j, (*i));
	}
}

void GLRenderDriver::setDepthBufferCheckEnabled(bool enabled)
{
	if (enabled)
	{
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void GLRenderDriver::setDepthBufferWriteEnabled(bool enabled)
{
	if (enabled)
	{
		glDepthMask(GL_TRUE);
	}
	else
	{
		glDepthMask(GL_FALSE);
	}	
}

void GLRenderDriver::setFog(FogMode mode, const Color& color, float density, float start, float end)
{
	GLint fogMode;
	switch (mode)
	{
	case FM_EXP:
		fogMode = GL_EXP;
		break;
	case FM_EXP2:
		fogMode = GL_EXP2;
		break;
	case FM_LINEAR:
		fogMode = GL_LINEAR;
		break;
	default:
		// disable fog
		glDisable(GL_FOG);
		return;
	}

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, fogMode);
	GLfloat fogColor[4] = {color.R, color.G, color.B, color.A};
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, density);
	glFogf(GL_FOG_START, start);
	glFogf(GL_FOG_END, end);
}

unsigned int GLRenderDriver::getNumTextureUnits()
{
	GLint units;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &units);

	return (unsigned int)units;
}

float GLRenderDriver::getMinimumDepthInputValue()
{
	// Range [-1.0f, 1.0f]
	return -1.0f;
}

float GLRenderDriver::getMaximumDepthInputValue()
{
	// Range [-1.0f, 1.0f]
	return 1.0f;
}

float GLRenderDriver::getHorizontalTexelOffset()
{
	// No offset in GL
	return 0.0f;
}
	  
float GLRenderDriver::getVerticalTexelOffset()
{
	// No offset in GL
	return 0.0f;
}

CGcontext GLRenderDriver::getCGContext()
{
	return mCgContext;
}

GLenum GLRenderDriver::getGLUsage(resource::BufferUsage usage)
{
	switch(usage)
	{
	case resource::BU_STATIC:
	case resource::BU_STATIC_WRITE_ONLY:
		return GL_STATIC_DRAW_ARB;
	case resource::BU_DYNAMIC:
	case resource::BU_DYNAMIC_WRITE_ONLY:
		return GL_DYNAMIC_DRAW_ARB;
	case resource::BU_DYNAMIC_WRITE_ONLY_DISCARDABLE:
		return GL_STREAM_DRAW_ARB;
	default:
		return GL_DYNAMIC_DRAW_ARB;
	};
}

void GLRenderDriver::makeGLMatrix(GLfloat gl_matrix[16], const core::matrix4& m)
{
	gl_matrix[ 0] = m[ 0];
	gl_matrix[ 1] = m[ 4];
	gl_matrix[ 2] = m[ 8];
	gl_matrix[ 3] = m[12];

	gl_matrix[ 4] = m[ 1];
	gl_matrix[ 5] = m[ 5];
	gl_matrix[ 6] = m[ 9];
	gl_matrix[ 7] = m[13];

	gl_matrix[ 8] = m[ 2];
	gl_matrix[ 9] = m[ 6];
	gl_matrix[10] = m[10];
	gl_matrix[11] = m[14];

	gl_matrix[12] = m[ 3];
	gl_matrix[13] = m[ 7];
	gl_matrix[14] = m[11];
	gl_matrix[15] = m[15];
}

void GLRenderDriver::setGLLight(unsigned int index, Light* light)
{
	GLint gl_index = GL_LIGHT0 + index;

	if (light->isVisible())
	{
		switch (light->getLightType())
		{
		case LIGHT_TYPE_SPOTLIGHT:
			glLightf(gl_index, GL_SPOT_CUTOFF, light->getSpotlightOuterAngle());
			glLightf(gl_index, GL_SPOT_EXPONENT, light->getSpotlightFalloff());
			break;
		default:
			glLightf(gl_index, GL_SPOT_CUTOFF, 180.0f);
			break;
		}

		// Color
		Color col;
		GLfloat f4vals[4];

		col = light->getAmbientColor();
		f4vals[0] = 0;//col.R;
		f4vals[1] = 0;//col.G;
		f4vals[2] = 0;//col.B;
		f4vals[3] = 1;//col.A;
		glLightfv(gl_index, GL_AMBIENT, f4vals);

		col = light->getDiffuseColor();
		f4vals[0] = col.R;
		f4vals[1] = col.G;
		f4vals[2] = col.B;
		f4vals[3] = col.A;
		glLightfv(gl_index, GL_DIFFUSE, f4vals);
	
		col = light->getSpecularColor();
		f4vals[0] = col.R;
		f4vals[1] = col.G;
		f4vals[2] = col.B;
		f4vals[3] = col.A;
		glLightfv(gl_index, GL_SPECULAR, f4vals);

		setGLLightPositionDirection(index, light);

		// Attenuation
		glLightf(gl_index, GL_CONSTANT_ATTENUATION, light->getAttenuationConstant());
		glLightf(gl_index, GL_LINEAR_ATTENUATION, light->getAttenuationLinear());
		glLightf(gl_index, GL_QUADRATIC_ATTENUATION, light->getAttenuationQuadric());
		// Enable in the scene
		glEnable(gl_index);

	}
	else
	{
		// Disable in the scene
		glDisable(gl_index);
	}
}

void GLRenderDriver::setGLLightPositionDirection(unsigned int index, Light* light)
{
	if (light == NULL)
		return;
	
	if (light->getGameObject() == NULL)
		return;

	game::Transform* pTransform = static_cast<game::Transform*>(light->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
	if (pTransform != NULL)
	{
		GLint gl_index = GL_LIGHT0 + index;

		// Set position / direction
		core::vector3d vec;
		GLfloat f4vals[4];
    
		vec = pTransform->getAbsolutePosition();
		f4vals[0] = vec.X;
		f4vals[1] = vec.Y;
		f4vals[2] = vec.Z;
		f4vals[3] = 1.0;
		glLightfv(gl_index, GL_POSITION, f4vals);    
    
		if (light->getLightType() == LIGHT_TYPE_SPOTLIGHT)
		{
			vec = pTransform->getAbsoluteOrientation() * core::vector3d::NEGATIVE_UNIT_Z;
			f4vals[0] = vec.X; 
			f4vals[1] = vec.Y;
			f4vals[2] = vec.Z; 
			f4vals[3] = 0.0; 
			glLightfv(gl_index, GL_SPOT_DIRECTION, f4vals);
		}
	}
}

void GLRenderDriver::initializeImpl()
{
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		MessageBox(NULL, "Can't Initialize GLew.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Check for hardware mipmapping support.
	if(GLEW_VERSION_1_4 || GLEW_SGIS_generate_mipmap)
	{
		//setCapability(RSC_AUTOMIPMAP);
	}

	// Check for blending support
	if(GLEW_VERSION_1_3 || GLEW_ARB_texture_env_combine || GLEW_EXT_texture_env_combine)
	{
		//setCapability(RSC_BLENDING);
	}

	// Check for Anisotropy support
	if(GLEW_EXT_texture_filter_anisotropic)
	{
		//setCapability(RSC_ANISOTROPY);
	}

	// Check for DOT3 support
	if(GLEW_VERSION_1_3 || GLEW_ARB_texture_env_dot3 ||	GLEW_EXT_texture_env_dot3)
	{
		//setCapability(RSC_DOT3);
	}

	// Check for cube mapping
	if(GLEW_VERSION_1_3 || GLEW_ARB_texture_cube_map || GLEW_EXT_texture_cube_map)
	{
		//setCapability(RSC_CUBEMAPPING);
	}

	// Point sprites
	if (GLEW_VERSION_2_0 ||	GLEW_ARB_point_sprite)
	{
		//setCapability(RSC_POINT_SPRITES);
	}
	// Check for point parameters
	if(GLEW_VERSION_1_4 || GLEW_ARB_point_parameters || GLEW_EXT_point_parameters)
	{
		//setCapability(RSC_POINT_EXTENDED_PARAMETERS);
	}

	// Check for VBO support
	if(GLEW_VERSION_1_5 || GLEW_ARB_vertex_buffer_object)
	{
		// Some buggy driver claim that it is GL 1.5 compliant and
		// not support ARB_vertex_buffer_object
		if (!GLEW_ARB_vertex_buffer_object)
		{
			// Assign ARB functions same to GL 1.5 version since
			// interface identical
			glBindBufferARB = glBindBuffer;
			glBufferDataARB = glBufferData;
			glBufferSubDataARB = glBufferSubData;
			glDeleteBuffersARB = glDeleteBuffers;
			glGenBuffersARB = glGenBuffers;
			glGetBufferParameterivARB = glGetBufferParameteriv;
			glGetBufferPointervARB = glGetBufferPointerv;
			glGetBufferSubDataARB = glGetBufferSubData;
			glIsBufferARB = glIsBuffer;
			glMapBufferARB = glMapBuffer;
			glUnmapBufferARB = glUnmapBuffer;
		}

		//setCapability(RSC_VBO);
	}

	mCgContext = cgCreateContext();
	
	CGerror error = cgGetError();
	if (error != CG_NO_ERROR)
	{
		std::string message = "Unable to destroy Cg context: " + std::string(cgGetErrorString(error));
		if (error == CG_COMPILER_ERROR)
		{
			// Get listing with full compile errors
			message += "\n" + std::string(cgGetLastListing(mCgContext));
		}
		core::Log::getInstance().logMessage("RenderSystem", message, core::LOG_LEVEL_ERROR);

		return;
	}
	cgSetParameterSettingMode(mCgContext, CG_DEFERRED_PARAMETER_SETTING);
	cgGLSetManageTextureParameters(mCgContext,  true);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing

	glDepthMask(GL_TRUE);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);				// Use The Good Calculations
	glEnable(GL_LINE_SMOOTH);							// Enable Anti-Aliasing

	for (unsigned int i = 0; i < mDisabledTexUnitsFrom; ++i)
	{
		glActiveTextureARB(GL_TEXTURE0 + i);
		glDisable(GL_TEXTURE_2D);		
	}

	glDisable(GL_LIGHTING);								// Disable Lighting
	glDisable(GL_BLEND);								// Turn Blending Off
	glDisable(GL_FOG);									// Turn Fog Off
	glColor4f(1.0f,1.0f,1.0f,1.0f);						// Set Color to initial value
}

void GLRenderDriver::uninitializeImpl()
{
	if (mCgContext != NULL)
		cgDestroyContext(mCgContext);
}

void GLRenderDriver::updateImpl(float elapsedTime) {}

GLint GLRenderDriver::getGLBlendMode(SceneBlendFactor gameBlend)
{
	switch(gameBlend)
	{
	case SBF_ONE:
		return GL_ONE;
	case SBF_ZERO:
		return GL_ZERO;
	case SBF_DEST_COLOR:
		return GL_DST_COLOR;
	case SBF_SOURCE_COLOR:
		return GL_SRC_COLOR;
	case SBF_ONE_MINUS_DEST_COLOR:
		return GL_ONE_MINUS_DST_COLOR;
	case SBF_ONE_MINUS_SOURCE_COLOR:
		return GL_ONE_MINUS_SRC_COLOR;
	case SBF_DEST_ALPHA:
		return GL_DST_ALPHA;
	case SBF_SOURCE_ALPHA:
		return GL_SRC_ALPHA;
	case SBF_ONE_MINUS_DEST_ALPHA:
		return GL_ONE_MINUS_DST_ALPHA;
	case SBF_ONE_MINUS_SOURCE_ALPHA:
		return GL_ONE_MINUS_SRC_ALPHA;
	};

	return GL_ONE;
}

GLenum GLRenderDriver::getGLType(ShaderType type)
{
	switch(type)
	{
	case SHADER_TYPE_VERTEX:
		return GL_VERTEX_PROGRAM_ARB;
	case SHADER_TYPE_FRAGMENT:
		return GL_FRAGMENT_PROGRAM_ARB;
	case SHADER_TYPE_GEOMETRY:
		return GL_GEOMETRY_PROGRAM_NV;
	default:
		return GL_VERTEX_PROGRAM_ARB;
	}
}

CGGLenum GLRenderDriver::getCGGLType(ShaderType type)
{
	switch(type)
	{
	case SHADER_TYPE_VERTEX:
		return CG_GL_VERTEX;
	case SHADER_TYPE_FRAGMENT:
		return CG_GL_FRAGMENT;
	case SHADER_TYPE_GEOMETRY:
		return CG_GL_GEOMETRY;
	default:
		return CG_GL_VERTEX;
	}
}

GLenum GLRenderDriver::getGLType(VertexElementType type)
{
	switch(type)
	{
	case VET_FLOAT1:
	case VET_FLOAT2:
	case VET_FLOAT3:
	case VET_FLOAT4:
		return GL_FLOAT;
	case VET_SHORT1:
	case VET_SHORT2:
	case VET_SHORT3:
	case VET_SHORT4:
		return GL_SHORT;
	case VET_COLOR:
		return GL_UNSIGNED_BYTE;
	default:
		return 0;
	};
}

bool GLRenderDriver::checkForCgError(CGcontext context)
{
	if (context == NULL) return false;

	CGerror error = cgGetError();
	if (error == CG_NO_ERROR) return false;

#ifdef _DEBUG
	std::string msg(cgGetErrorString(error));
	if (error == CG_COMPILER_ERROR)
	{
		// Get listing with full compile errors
		msg = msg + "\n" + cgGetLastListing(context);
	}

	std::cout<<msg<<std::endl;
#endif

	return true;
}

GLRenderDriver& GLRenderDriver::getInstance()
{
	return core::Singleton<GLRenderDriver>::getInstance();
}

GLRenderDriver* GLRenderDriver::getInstancePtr()
{
	return core::Singleton<GLRenderDriver>::getInstancePtr();
}

} // end namespace render

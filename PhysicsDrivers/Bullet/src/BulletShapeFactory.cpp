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

#include <BulletShapeFactory.h>
#include <BulletShape.h>

namespace physics
{

Shape* BulletShapeFactory::createShape(ShapeType type)
{
	Shape* newShape = nullptr;
	
	switch(type)
	{
	case SHAPE_TYPE_UNDEFINED:
		newShape = nullptr;
		break;
	case SHAPE_TYPE_PLANE:
		newShape = new BulletPlaneShape();
		break;
	case SHAPE_TYPE_SPHERE:
		newShape = new BulletSphereShape();
		break;
	case SHAPE_TYPE_BOX:
		newShape = new BulletBoxShape();
		break;
	case SHAPE_TYPE_CAPSULE:
		newShape = nullptr;
		break;
	case SHAPE_TYPE_CONVEX:
		newShape = nullptr;
		break;
	case SHAPE_TYPE_MESH:
		newShape = nullptr;
		break;
	default:
		newShape = nullptr;
		break;	
	}
		
	return newShape;
}

void BulletShapeFactory::destroyShape(Shape* shape)
{
	if (shape == nullptr)
		return;

	switch(shape->getShapeType())
	{
	case SHAPE_TYPE_UNDEFINED:
		{
			if (shape != nullptr)
			{
				delete shape;
			}
		}
		break;
	case SHAPE_TYPE_PLANE:
		{
			BulletPlaneShape* bulletShape = static_cast<BulletPlaneShape*>(shape);
			assert(bulletShape != nullptr);
			if (bulletShape != nullptr)
			{
				delete bulletShape;
			}
		}
		break;
	case SHAPE_TYPE_SPHERE:
		{
			BulletSphereShape* bulletShape = static_cast<BulletSphereShape*>(shape);
			assert(bulletShape != nullptr);
			if (bulletShape != nullptr)
			{
				delete bulletShape;
			}
		}
		break;
	case SHAPE_TYPE_BOX:
		{
			BulletBoxShape* bulletShape = static_cast<BulletBoxShape*>(shape);
			assert(bulletShape != nullptr);
			if (bulletShape != nullptr)
			{
				delete bulletShape;
			}
		}
		break;
	case SHAPE_TYPE_CAPSULE:
		{
			if (shape != nullptr)
			{
				delete shape;
			}
		}
		break;
	case SHAPE_TYPE_CONVEX:
		{
			if (shape != nullptr)
			{
				delete shape;
			}
		}
		break;
	case SHAPE_TYPE_MESH:
		{
			if (shape != nullptr)
			{
				delete shape;
			}
		}
		break;
	default:
		{
			if (shape != nullptr)
			{
				delete shape;
			}
		}
		break;	
	}
}

} // end namespace physics

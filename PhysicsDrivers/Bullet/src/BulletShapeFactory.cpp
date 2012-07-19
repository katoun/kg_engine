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

#include <BulletShapeFactory.h>
#include <BulletShape.h>

namespace physics
{

Shape* BulletShapeFactory::createShape(ShapeType type)
{
	Shape* newShape = NULL;
	
	switch(type)
	{
	case SHAPE_TYPE_UNDEFINED:
		newShape = NULL;
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
		newShape = NULL;
		break;
	case SHAPE_TYPE_CONVEX:
		newShape = NULL;
		break;
	case SHAPE_TYPE_MESH:
		newShape = NULL;
		break;
	default:
		newShape = NULL;
		break;	
	}
		
	return newShape;
}

void BulletShapeFactory::destroyShape(Shape* shape)
{
	if (shape == NULL)
		return;

	switch(shape->getShapeType())
	{
	case SHAPE_TYPE_UNDEFINED:
		{
			if (shape != NULL)
			{
				delete shape;
			}
		}
		break;
	case SHAPE_TYPE_PLANE:
		{
			BulletPlaneShape* bulletShape = static_cast<BulletPlaneShape*>(shape);
			assert(bulletShape != NULL);
			if (bulletShape != NULL)
			{
				delete bulletShape;
			}
		}
		break;
	case SHAPE_TYPE_SPHERE:
		{
			BulletSphereShape* bulletShape = static_cast<BulletSphereShape*>(shape);
			assert(bulletShape != NULL);
			if (bulletShape != NULL)
			{
				delete bulletShape;
			}
		}
		break;
	case SHAPE_TYPE_BOX:
		{
			BulletBoxShape* bulletShape = static_cast<BulletBoxShape*>(shape);
			assert(bulletShape != NULL);
			if (bulletShape != NULL)
			{
				delete bulletShape;
			}
		}
		break;
	case SHAPE_TYPE_CAPSULE:
		{
			if (shape != NULL)
			{
				delete shape;
			}
		}
		break;
	case SHAPE_TYPE_CONVEX:
		{
			if (shape != NULL)
			{
				delete shape;
			}
		}
		break;
	case SHAPE_TYPE_MESH:
		{
			if (shape != NULL)
			{
				delete shape;
			}
		}
		break;
	default:
		{
			if (shape != NULL)
			{
				delete shape;
			}
		}
		break;	
	}
}

} // end namespace physics

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

#ifndef _BULLET_BODY_FACTORY_H_
#define _BULLET_BODY_FACTORY_H_

#include <BulletConfig.h>
#include <physics/BodyFactory.h>

namespace physics
{

class Body;
class BodyData;

class BULLET_PUBLIC_EXPORT BulletBodyFactory: public BodyFactory
{
public:

	Body* createBody(BodyData* bodyData);
	Body* createBody(const std::string& name, BodyData* bodyData);

	void destroyBody(Body* body);
};

} // end namespace physics

#endif
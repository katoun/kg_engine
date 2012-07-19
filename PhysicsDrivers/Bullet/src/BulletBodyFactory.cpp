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

#include <BulletBodyFactory.h>
#include <BulletBody.h>

namespace physics
{

Body* BulletBodyFactory::createBody(BodyData* bodyData)
{
	return new BulletBody(bodyData);
}

Body* BulletBodyFactory::createBody(const std::string& name, BodyData* bodyData)
{
	return new BulletBody(name, bodyData);
}

void BulletBodyFactory::destroyBody(Body* body)
{
	BulletBody* bulletBody = static_cast<BulletBody*>(body);

	assert(bulletBody != NULL);
	if (bulletBody != NULL) delete bulletBody;
}

} // end namespace physics

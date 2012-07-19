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

#ifndef _BODY_SERIALIZER_H_
#define _BODY_SERIALIZER_H_

#include <Config.h>
#include <resource/Serializer.h>

#include <string>
#include <map>

namespace resource
{

//! Class for serializing Bodies to / from a .body script.
class BodySerializer : public Serializer
{
public:
	
	BodySerializer();
	virtual ~BodySerializer();

	//! Imports a Body from an .xml file.
	bool importResource(Resource* dest, const std::string& filename);

	//! Exports a body to the file specified. 
	bool exportResource(Resource* source, const std::string& filename);
};

}// end namespace resource

#endif
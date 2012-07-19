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

#ifndef _MESH_SERIALIZER_H_
#define _MESH_SERIALIZER_H_

#include <Config.h>
#include <resource/Serializer.h>

#include <string>

namespace resource
{

//! Class for serialising mesh data to/from an .mesh file.
//!
//! This class allows exporters to write .mesh files easily, and allows the
//! engine to import .mesh files into instatiated Meshes.
class MeshSerializer: public Serializer
{
public:

	MeshSerializer();
	virtual ~MeshSerializer();

	//! Imports a MeshData from an .xml file.
	bool importResource(Resource* dest, const std::string& filename);

	//! Exports a mesh to the file specified. 
	//!
	//! This method takes an externally created MeshData, and exports it to a .mesh file.
	//! \param meshData: Pointer to the MeshData to export
	//! \param filename: The destination filename.
	bool exportResource(Resource* source, const std::string& filename);
};

}// end namespace resource

#endif
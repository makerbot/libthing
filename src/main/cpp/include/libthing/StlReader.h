
#ifndef _STL_READER_H_
#define _STL_READER_H_ (1)

#include "libthing/Mesh.h"
#include "libthing/PlatformTools.h"

namespace libthing {



class StlReader
{
	// NOTE: for stl legacy read-in reasons, we are using floats here,
	// instead of our own Scalar type
	struct vertexes_t {
		float nx, ny, nz;
		float x1, y1, z1;
		float x2, y2, z2;
		float x3, y3, z3;
		uint16_t attrBytes;
	};

	union {
		struct vertexes_t vertexes;
		uint8_t bytes[sizeof(vertexes_t)];
	} tridata;

	union
	{
		uint32_t intval;
		uint16_t shortval;
		uint8_t bytes[4];
	} intdata;

	//// Read a meshin
	size_t meshFromStlFile(SmartMesh &mesh, char const *filename);
};


}; //end namespace

#endif /*_STL_READER_H_*/

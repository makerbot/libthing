
#include "libthing/StlWriter.h"
#include <sstream>


using namespace std;
using namespace libthing;


void StlWriter::open(const char* fileName, const char *solid)

{
	solidName = solid;
	out.open(fileName);
	if(!out)
	{
		std::stringstream ss;
		ss << "Can't open \"" << fileName << "\"";
		//MeshyException problem(ss.str().c_str());
		//throw (problem);
	}

	// bingo!
	out << std::scientific;
	out << "solid " << solidName << std::endl;

}

void StlWriter::writeTriangleAscii(const Triangle3& t)
{
	// normalize( (v1-v0) cross (v2 - v0) )
	// y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x

	Vector3 n = t.normal();
	out << " facet normal " << n[0] << " " << n[1] << " " << n[2] << std::endl;
	out << "  outer loop"<< std::endl;
	out << "    vertex " << t[0].x << " " << t[0].y << " " << t[0].z << std::endl;
	out << "    vertex " << t[1].x << " " << t[1].y << " " << t[1].z << std::endl;
	out << "    vertex " << t[2].x << " " << t[2].y << " " << t[2].z << std::endl;
	out << "  endloop" << std::endl;
	out << " endfacet" << std::endl;
}

void StlWriter::close()
{
	out << "endsolid " << solidName << std::endl;
	out.close();
}

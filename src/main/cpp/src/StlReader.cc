
#include "libthing/StlReader.h"
#include "libthing/PlatformTools.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <string.h>


using namespace std;
using namespace libthing;

size_t StlReader::meshFromStlFile(SmartMesh &mesh, char const *filename)
{
	size_t facecount = 0;


	uint8_t buf[512];
	FILE *fHandle = fopen(filename, "rb");
	if (!fHandle)
	{
		string msg = "Can't open \"";
		msg += filename;
		msg += "\". Check that the file name is correct and that you have sufficient privileges to open it.";
		//MeshyException problem(msg.c_str());
		//throw (problem);
	}

	if (fread(buf, 1, 5, fHandle) < 5) {
		string msg = "\"";
		msg += filename;
		msg += "\" is empty!";
		//MeshyException problem(msg.c_str());
		//throw (problem);
	}
	bool isBinary = true;
	if (!strncasecmp((const char*) buf, "solid", 5)) {
		isBinary = false;
	}
	if (isBinary) {
		// Binary STL file
		// Skip remainder of 80 character comment field
		if (fread(buf, 1, 75, fHandle) < 75) {
			string msg = "\"";
			msg += filename;
			msg += "\" is not a valid stl file";
			//MeshyException problem(msg.c_str());
			//throw (problem);
		}
		// Read in triangle count
		if (fread(intdata.bytes, 1, 4, fHandle) < 4) {
			string msg = "\"";
			msg += filename;
			msg += "\" is not a valid stl file";
			//MeshyException problem(msg.c_str());
			//throw (problem);
		}
		convertFromLittleEndian32(intdata.bytes);
		uint32_t tricount = intdata.intval;
		int countdown = (int)tricount;
		while (!feof(fHandle) && countdown-- > 0) {
			if (fread(tridata.bytes, 1, 3 * 4 * 4 + 2, fHandle) < 3 * 4 * 4 + 2) {
                //Log::often() << __FUNCTION__ << "BREAKING" << endl;
				break;
			}
			for (int i = 0; i < 3 * 4; i++) {
				convertFromLittleEndian32(tridata.bytes + i * 4);
			}
			convertFromLittleEndian16((uint8_t*) &tridata.vertexes.attrBytes);

			vertexes_t &v = tridata.vertexes;
			Vector3 pt1(v.x1, v.y1, v.z1);
			Vector3 pt2(v.x2, v.y2, v.z2);
			Vector3 pt3(v.x3, v.y3, v.z3);

			Triangle3 triangle(pt1, pt2, pt3);
			mesh.addTriangle(triangle);

			facecount++;
		}

		/// Throw removed to continue coding progress. We may not expect all
		/// triangles to load, depending on situation. Needs debugging/revision
		if(mesh.triangleCount() != tricount) {
			string msg = "Warning: triangle count err in \"";
			msg += filename;
			msg += "\".  Expected: ";
			msg += stringify((size_t)tricount);
			msg += ", Read:";
			msg += stringify(mesh.triangleCount());
			msg += ", faced:";
			msg += stringify(facecount);
            //Log::often() << msg;
//			MeshyException problem(msg.c_str());
//			throw (problem);
		}


	} else {
		// ASCII STL file
		// Gobble remainder of solid name line.
		fgets((char*) buf, sizeof(buf), fHandle);
		while (!feof(fHandle)) {
			fscanf(fHandle, "%80s", buf);
			if (!strcasecmp((char*) buf, "endsolid")) {
				break;
			}
			vertexes_t &v = tridata.vertexes;
			bool success = true;
			if (fscanf(fHandle, "%*s %f %f %f", &v.nx, &v.ny, &v.nz) < 3)
				success = false;
			if (fscanf(fHandle, "%*s %*s") < 0)
				success = false;
			if (fscanf(fHandle, "%*s %f %f %f", &v.x1, &v.y1, &v.z1) < 3)
				success = false;
			if (fscanf(fHandle, "%*s %f %f %f", &v.x2, &v.y2, &v.z2) < 3)
				success = false;
			if (fscanf(fHandle, "%*s %f %f %f", &v.x3, &v.y3, &v.z3) < 3)
				success = false;
			if (fscanf(fHandle, "%*s")< 0)
				success = false;
			if (fscanf(fHandle, "%*s")< 0)
				success = false;
			if(!success)
			{
				stringstream msg;
				//msg << "Error reading face " << facecount << " in file \"" << filename << "\"";
				//MeshyException problem(msg.str().c_str());
                //Log::often() << msg << endl;
                //Log::often() << buf << endl;
				//throw(problem);
			}
			Triangle3 triangle(Vector3(v.x1, v.y1, v.z1),	Vector3(v.x2, v.y2, v.z2),	Vector3(v.x3, v.y3, v.z3));
			mesh.addTriangle(triangle);

			facecount++;
		}
	}
	fclose(fHandle);
	return mesh.triangleCount();

}

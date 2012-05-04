#include <list>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string.h>
#include "libthing/ObjFormatter.h"

//#include <QVector3D>
//#include <QString>
//#include <QList>
//#include <QTextStream>

using namespace libthing;
using namespace std;

#define MAX_LINE 4096

Mesh* ObjFormatter::readMesh(istream& in) {

    //    QList<QVector3D> vertexList;
    std::vector<Vector3> vertexList;
    std::vector<Vector3> normalList;
    //    QList<QVector3D> normalList;
    VertexNormalTriangleMesh* mesh = new VertexNormalTriangleMesh();
    //QTextStream in(&inf);
    char buffer[MAX_LINE];
    //    while (!in.atEnd()) {
    while( ! in.eof() ) {
        //        QString line = in.readLine(MAX_LINE);
        in.getline(buffer, MAX_LINE);
        //        if (line.isEmpty() || line[0] == '#') continue;
        if(strlen(buffer) == 0 || buffer[0] == '#')
            continue;
        std::string line(buffer);
        std::stringstream ss(line);
        std::istream_iterator<std::string> begin(ss), end;
        std::vector<std::string> parts(begin,end);
        //        QStringList parts = line.simplified().split(" ");
        if (parts.size() == 0)
            continue;
        if (strncmp(parts[0].c_str(),"v",2) == 0 ) {
            // vertex
            if (parts.size() < 4) {
                throw -1; //ParseException("Insufficient coordinates for vertex");
            }
            double v0,v1,v2;
            v0 = atof(parts[1].c_str());
            v1 = atof(parts[2].c_str());
            v2 = atof(parts[3].c_str());
            vertexList.push_back(Vector3(v0,v1,v2));
            //	vertexList.append(QVector3D(parts[1].toDouble(),parts[2].toDouble(),parts[3].toDouble()));
        } else if (strncmp(parts[0].c_str(),"vn",2) ) {
            //            // vertex normal
            if (parts.size() < 4) {
                throw -1; //ParseException("Insufficient coordinates for vertex");
            }
            double v0,v1,v2;
            v0 = atof(parts[1].c_str());
            v1 = atof(parts[2].c_str());
            v2 = atof(parts[3].c_str());
            normalList.push_back(Vector3(v0,v1,v2));
            //QVector3D normal(parts[1].toDouble(),parts[2].toDouble(),parts[3].toDouble());
            //normalList.append(normal.normalized());
        } else if (strncmp(parts[0].c_str(),"f",1) ) {
			// face
			bool needsNormals = false; // If set, we need to compute normals for the face
			if (parts.size() < 4)
				throw -1; //ParseException("Insufficient vertices for face");
            // Fan triangulation. If .obj files can contain non-convex faces, this will cause
            // some errors.
            for (int j = 0; j < parts.size()-3; j++) {
	            VertexNormTriangle3 t;
	            for (int i = 0; i < 3; i++) {
					// Since we're doing a fan triangulation, we need to ensure
		            // that the first vertex of every triangle is idx 0.
		            int partIdx = (i==0)?1:(1+i+j);
					//QStringList vparts = parts[partIdx].split("/");
					std::stringstream ss_sub(parts[partIdx]);
					std::istream_iterator<std::string> begin(ss_sub),end;
					std::vector<std::string> vparts(begin,end);
					int vidx = atoi(vparts[0].c_str());
					if (vidx > vertexList.size() || vidx == 0) {
						throw -1; // ParseException("Face requires unspecified vertex");
					}
    		       	t[i] = vertexList[vidx-1];
					if (vparts.size() >= 3) {
            			int nidx = atoi(vparts[2].c_str());
							if (nidx > normalList.size() || nidx == 0) {
								throw -1; //ParseException("Face requires unspecified normal");
							}
							t.setVertexNormal(i,normalList[nidx-1]);
                    } else if (normalList.size() >= vidx) {
                        // Implicit vertex normal
                        t.setVertexNormal(i, normalList[vidx-1]);
                    } else {
                        needsNormals = true;
                    }
				}
                if (needsNormals) {
                    Vector3 n = t.normal();
                    for (int i = 0; i < 3; i++) {
                        t.setVertexNormal(i, n);
                    }
            }
            	mesh->addTriangle(t);
           }
       }
	}
	
    //qDebug() << "Loaded OBJ mesh; facet count" << mesh->tris().size() << "vertices" << vertexList.size() << "normals" << normalList.size();
    return mesh;
}

void ObjFormatter::writeMesh(ostream& outFh, const Mesh& mesh)
{
	FaceNormalTriangleMesh* fntMesh = dyamic_cast<FaceNormalTriangleMesh>(mesh);
	VertexNormalTriangleMesh* vntMesh = dyamic_cast<VertexNormalTriangleMesh>(mesh);
	
	if(vntMesh != NULL) {
		std::vector<VertexNormTriangle3> tris = vntMesh.readAllTriangles();
		

	throw -1;
}

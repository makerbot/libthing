#include "libthing/StlFormatters.h"
#include "libthing/Mesh.h"
//#include "objtree/meshnode.h"
//
//#include <QVector3D>
//#include <QString>
//#include <QTextStream>

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace libthing;
//
//static void expect(QTextStream& in, const QString& expected) {
//    in.skipWhiteSpace();
//    QString got = in.read(expected.length());
//    if (got != expected) {
//        throw ParseException(QString("Expected ") + expected);
//    }
//    in.skipWhiteSpace();
//}
//
//static QVector3D readVertex(QTextStream& in, const QString& expected) {
//    expect(in,expected);
//    float x, y, z;
//    in >> x >> y >> z;
//    return QVector3D(x,y,z);
//}
static void expect(std::istream& in, const string expected) {
  string got;
  in >> got;
  if (got != expected) {
    throw ParseException();
  }
  in >> std::ws;
}


static Vector3 readVertex(std::istream& in, const string expected) {
  expect(in,expected);
  Vector3 v;
  in >> v[0] >> v[1] >> v[2];
  return v;
}
static string readHeader(std::istream& in) {
  expect(in,"solid");
  string comment;
  getline(in,comment);
  in >> std::ws;
  return comment;
}


static void readTriangle(istream& in, FaceNormalTriangleMesh& mesh) {
    Vector3 v0,v1,v2,n0;

    n0 = readVertex(in,"normal");

    expect(in, "outer");
    expect(in, "loop");
    v0 = readVertex(in,"vertex");
    v1 = readVertex(in,"vertex");
    v2 = readVertex(in,"vertex");
    expect(in, "endloop");
    expect(in, "endfacet");

    mesh.addTriangle( FaceNormTriangle3(v0,v1,v2,n0) );
}


#define MAX_COLS 80

Mesh* AsciiStlFormatter::readMesh(istream& in) {
    FaceNormalTriangleMesh* mesh = new FaceNormalTriangleMesh();

    in.width(MAX_COLS);
    string comment = readHeader(in);
    //cout << "STL ASCII header comment: " << comment << endl;
    mesh->setComment(comment);

//    QTextStream in(&inf);
//    QString lineStr = in.readLine(MAX_LINE);
//    QTextStream line(&lineStr);
//    line.skipWhiteSpace();
//    QString word;
	std::string word;
//    line >> word;
//    if (word != "solid") {
//        throw ParseException("ASCII STL did not start with 'solid'");
//    }

//    QString name = line.readAll().trimmed();
    while (1) {
        in >> word;
        if (word == "facet") {
            readTriangle(in,*mesh);
        } else if (word == "endsolid") {
            break;
        } else {
            throw -1 ; //ParseException("Expected endsolid or facet");
        }
    }
//    //cout << "Loaded mesh; facet count " << mesh.tris.size() << endl;
    return mesh;
}


static void writeTriangle(ostream& out, const Triangle3& tri, const Vector3& normal) {
    out << " facet normal " << normal.x << " " << normal.y << " " << normal.z << "\n";
    out << "  outer loop"<< "\n";
    out << "    vertex " << tri[0].x << " " << tri[0].y << " " << tri[0].z << "\n";
    out << "    vertex " << tri[1].x << " " << tri[1].y << " " << tri[1].z << "\n";
    out << "    vertex " << tri[2].x << " " << tri[2].y << " " << tri[2].z << "\n";
    out << "  endloop" << "\n";
    out << " endfacet" << "\n";
}


//static void writeTransformedTriangle(ostream& out, const Triangle& tri, const QMatrix4x4& trans) {
//    Triangle t;
//    t.p[0] = trans*tri.p[0];
//    t.p[1] = trans*tri.p[1];
//    t.p[2] = trans*tri.p[2];
//    writeTriangle(out, t, t.computeNormal());
//}

#define VAR(V,init) __typeof(init) V=(init)
#define FOREACH(I,C) for(VAR(I,(C).begin());I!=(C).end();I++)


// writes out a single, untransformed mesh
void AsciiStlFormatter::writeMesh(ostream& out, const Mesh& mesh) {

    const FaceNormalTriangleMesh* fntMesh = dynamic_cast<const FaceNormalTriangleMesh*>(&mesh);
    const VertexNormalTriangleMesh* vntMesh = dynamic_cast<const VertexNormalTriangleMesh*>(&mesh);


    out << "solid " << mesh.getComment() << endl;

    if(fntMesh != 0){
    	const std::vector<FaceNormTriangle3>& tris = fntMesh->readAllTriangles();
    	FOREACH(I, tris) { //FUTURE: make this a C++11 foreach someday soon
			writeTriangle(out, *I, (*I).normal());
    	}
    }
    else if(vntMesh != 0) {
    	const std::vector<VertexNormTriangle3>& tris = vntMesh->readAllTriangles();
    	FOREACH(I, tris) { //FUTURE: make this a C++11 foreach someday soon
			writeTriangle(out, *I, (*I).normal());
    	}
    }
    else // We should have better failure behavior here.
        throw -1;

    out << "endsolid " << mesh.getComment() << endl;
}



//// this could probably be turned into a loop
//// this does the DFT for writeMesh(QFile, SceneNode)
//void writeMeshRecurse(QTextStream& out, const SceneNode* node, const QMatrix4x4& transformation) {
//
//    QMatrix4x4 newTrans = transformation;
//
//    const TransformNode* transNode = dynamic_cast<const TransformNode*>(node);
//    if(transNode != 0)
//        newTrans *= transNode->matrix();
//
//    const MeshNode* meshNode = dynamic_cast<const MeshNode*>(node);
//    if(meshNode != 0) {
//
//        const FaceNormalTriangleMesh* fntMesh = 0;
//        const VertexNormalTriangleMesh* vntMesh = 0;
//        fntMesh = dynamic_cast<const FaceNormalTriangleMesh*>(&meshNode->mesh());
//        vntMesh = dynamic_cast<const VertexNormalTriangleMesh*>(&meshNode->mesh());
//
//        if(fntMesh != 0)
//            foreach(FaceNormalTriangle tri, fntMesh->tris())
//                writeTransformedTriangle(out, tri, transformation);
//        else if(vntMesh != 0)
//            foreach(VertexNormalTriangle tri, vntMesh->tris())
//                writeTransformedTriangle(out, tri, transformation);
//        else // We should have better failure behavior here.
//            throw -1;
//    }
//
//    foreach(SceneNode* sn, node->children()) {
//        writeMeshRecurse(out, sn, newTrans);
//    }
//}


//// writes out a scene tree, transforming each mesh and witing them all
//// out as the same solid
//void AsciiStlFormatter::writeMesh(QFile& outf, const SceneNode& node) {
//
//    // depth-first-traverse the scene tree
//    //   if node is a transform node add its transform onto our transformation stack
//    //   if node is a mesh node
//    //     loop triangles
//    //       transform each triangle, write it out
//
//    outf.open(QIODevice::WriteOnly | QIODevice::Text);
//
//    QMatrix4x4 transformation;
//    transformation.setToIdentity();
//
//    QTextStream out(&outf);
//
//    out << "solid " << "solidname" << "\n";
//
//    writeMeshRecurse(out, &node, transformation);
//
//    out << "endsolid " << "solidname" << "\n";
//    out.flush();
//}
//

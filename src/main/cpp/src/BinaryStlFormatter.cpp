#include <vector>
#include <algorithm>
#include <iostream>
#include <stdint.h>
#include <string.h>

#include "libthing/StlFormatters.h"
#include "libthing/Mesh.h"

//// I too, hate this kind of crap, but what can one do?
/// this is needed so network byte order works on all platforms.
/// There may be a cleaner way, so fix it if you know how....
#ifdef ON_DARWIN
#if !defined(le32toh) || !defined(htole32)
  #if BYTE_ORDER == LITTLE_ENDIAN
    #define le32toh(x) (x)
    #define htole32(x) bswap_32(x)
  #else
    #define le32toh(x) bswap_32(x)
    #define htole32(x) (x)
  #endif
#endif
 #if defined(HAVE_COREFOUNDATION_COREFOUNDATION_H)
    #include <CoreFoundation/CoreFoundation.h>
    #define htole32(x) CFSwapInt32HostToLittle(x)
    #define le32toh(x) CFSwapInt32LittleToHost(x)
 #endif
#else
  #if defined(ON_LINUX)
    #include <endian.h>
  #endif /*ON_LINUX*/
#endif

using namespace std;
using namespace libthing;

static int readUInt32(std::istream& in) {
  uint32_t tmp;
  in.read((istream::char_type*)&tmp,4);
  return le32toh(tmp);
}

static float readFloat(std::istream& in) {
  uint32_t tmp;
  in.read((istream::char_type*)&tmp,4);
  tmp = le32toh(tmp);
  return *(float*)(&tmp);
}

static Vector3 readVertex(std::istream& in) {
  Vector3 v;
  v[0] = readFloat(in);
  v[1] = readFloat(in);
  v[2] = readFloat(in);
  return v;
}

#define COMMENT_MAX_LEN 80

Mesh* BinaryStlFormatter::readMesh(std::istream& in) {
  FaceNormalTriangleMesh mesh;
  istream::char_type comment[81];
  istream::char_type padding[2];
  in.read(comment,COMMENT_MAX_LEN);
  comment[COMMENT_MAX_LEN] = '\0';
  //cout << "STL header comment: " << comment << endl;
  mesh.setComment(comment);
  uint32_t facets = readUInt32(in);
  //cout << "Facet count: " << facets << endl;
  for (int f = 0; f < facets && !in.eof(); f++) {
    // TODO: check that file is long enough! We can't have this
    // block.
    // Read normal
	Vector3 v0,v1,v2, f0;
    f0 = readVertex(in);
    v0 = readVertex(in);
    v1 = readVertex(in);
    v2 = readVertex(in);
    mesh.addTriangle(FaceNormTriangle3(v0,v1,v2,f0));

    // Read padding
    in.read(padding,2);
    //mesh.tris.push_back(t);
    //int triIdx = mesh.tris.size();
    //mesh.edges.addEdgesForTriangle(t,triIdx);
  }
  if (in.fail()) {
    cerr << "Error reading file; file possibly corrupted or misunderstood." <<
      endl;
    throw ParseException();
  }
  return &mesh;
}

//Mesh* BinaryStlFormatter::readMesh(QFile& inf) {
//    FaceNormalTriangleMesh* mesh = new FaceNormalTriangleMesh();
//    QDataStream in(&inf);
//    char comment[81];
//    // STL is in network byte order
//    in.setByteOrder(QDataStream::LittleEndian);
//    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
//    in.readRawData(comment,80);
//    comment[80] = '\0';
//    //qDebug() << "STL header comment:" << comment;
//    quint32 facets;
//    in >> facets;
//    //qDebug() << "Facet count: " << facets;
//    for (quint32 f = 0; f < facets; f++) {
//        // TODO: check that file is long enough! We can't have this
//        // block.
//        // Read normal
//        FaceNormalTriangle t;
//        t.n = readVertex(in);
//        for (int i =0; i < 3; i++) {
//            t.p[i] = readVertex(in);
//        }
//        mesh->addTri(t);
//        // Read padding
//        quint16 attributeByteCount;
//        in >> attributeByteCount;
//        if (in.status() == QDataStream::ReadPastEnd) {
//            //qDebug() << "Error reading file; file possibly corrupted or misunderstood.";
//            throw ParseException("Stream too short");
//        }
//    }
//    return mesh;
//}
//

//
//void writeBinReal32(QFile& out, float f) {
//    out.write((const char*)&f,4);
//}
//
//void writeBinQVector3D(QFile& out, const QVector3D& v) {
//    writeBinReal32(out,v.x());
//    writeBinReal32(out,v.y());
//    writeBinReal32(out,v.z());
//}
//
//static void writeBinTriangle(QFile& out, const Triangle& tri, const QVector3D& normal) {\
//    writeBinQVector3D(out,normal);
//    writeBinQVector3D(out,tri.p[0]);
//    writeBinQVector3D(out,tri.p[1]);
//    writeBinQVector3D(out,tri.p[2]);
//    uint16_t attr = 0;
//    out.write((const char*)&attr,2);
//}
//
//static void writeBinTransformedTriangle(QFile& out, const Triangle& tri, const QMatrix4x4& trans) {
//    Triangle t;
//    t.p[0] = trans*tri.p[0];
//    t.p[1] = trans*tri.p[1];
//    t.p[2] = trans*tri.p[2];
//    writeBinTriangle(out, t, t.computeNormal());
//}
//
//void writeBinMeshRecurse(QFile& out, const SceneNode* node, const QMatrix4x4& transformation) {
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
//                writeBinTransformedTriangle(out, tri, transformation);
//        else if(vntMesh != 0)
//            foreach(VertexNormalTriangle tri, vntMesh->tris())
//                writeBinTransformedTriangle(out, tri, transformation);
//        else // We should have better failure behavior here.
//            throw -1;
//    }
//
//    foreach(SceneNode* sn, node->children()) {
//        writeBinMeshRecurse(out, sn, newTrans);
//    }
//}
//
//uint32_t countTris(const SceneNode* node) {
//    const MeshNode* meshNode = dynamic_cast<const MeshNode*>(node);
//    if (meshNode != 0) {
//        const FaceNormalTriangleMesh* fntMesh = dynamic_cast<const FaceNormalTriangleMesh*>(&meshNode->mesh());
//        const VertexNormalTriangleMesh* vntMesh = dynamic_cast<const VertexNormalTriangleMesh*>(&meshNode->mesh());
//        if(fntMesh != 0)
//            return fntMesh->tris().size();
//        else if(vntMesh != 0)
//            return vntMesh->tris().size();
//        else // We should have better failure behavior here.
//            throw -1;
//    } else {
//        uint32_t tris = 0;
//        foreach(SceneNode* sn, node->children()) {
//            tris += countTris(sn);
//        }
//        return tris;
//    }
//}
//
void BinaryStlFormatter::writeMesh(std::ostream& outFh, const libthing::Mesh& mesh) {

    // depth-first-traverse the scene tree
    //   if node is a transform node add its transform onto our transformation stack
    //   if node is a mesh node
    //     loop triangles
    //       transform each triangle, write it out

//    QMatrix4x4 transformation;
//    transformation.setToIdentity();

//    char nameField[COMMENT_MAX_LEN];
//    memset(nameField,0,COMMENT_MAX_LEN);
//    string name = mesh.getComment();
//    strncpy(nameField,name.c_str(),name.size());
//    outFh.write(nameField, COMMENT_MAX_LEN);

	throw -1;

//    uint32_t triCount = countTris(&node);
//    outf.write((const char*)&triCount,sizeof(triCount));
//
//    writeBinMeshRecurse(outf, &node, transformation);

}

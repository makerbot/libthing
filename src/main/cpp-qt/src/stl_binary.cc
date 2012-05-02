#include "stl.h"
#include "objtree/transformnode.h"
#include "objtree/meshnode.h"

#include <iostream>
#include <stdint.h>

using namespace std;

static QVector3D readVertex(QDataStream& in) {
    float x, y, z;
    in >> x;
    in >> y;
    in >> z;
    return QVector3D(x,y,z);
}

SmartMesh* BinaryStlFormatter::readMesh(QFile& inf) {
    FaceNormalTriangleMesh* mesh = new FaceNormalTriangleMesh();
    QDataStream in(&inf);
    char comment[81];
    // STL is in network byte order
    in.setByteOrder(QDataStream::LittleEndian);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    in.readRawData(comment,80);
    comment[80] = '\0';
    //qDebug() << "STL header comment:" << comment;
    quint32 facets;
    in >> facets;
    //qDebug() << "Facet count: " << facets;
    for (quint32 f = 0; f < facets; f++) {
        // TODO: check that file is long enough! We can't have this
        // block.
        // Read normal
        FaceNormalTriangle t;
        t.n = readVertex(in);
        for (int i =0; i < 3; i++) {
            t.p[i] = readVertex(in);
        }
        mesh->addTri(t);
        // Read padding
        quint16 attributeByteCount;
        in >> attributeByteCount;
        if (in.status() == QDataStream::ReadPastEnd) {
            //qDebug() << "Error reading file; file possibly corrupted or misunderstood.";
            throw ParseException("Stream too short");
        }
    }
    return mesh;
}

void BinaryStlFormatter::writeMesh(QFile& out, const SmartMesh& mesh) {
    throw -1;
}

void writeBinReal32(QFile& out, float f) {
    out.write((const char*)&f,4);
}

void writeBinQVector3D(QFile& out, const QVector3D& v) {
    writeBinReal32(out,v.x());
    writeBinReal32(out,v.y());
    writeBinReal32(out,v.z());
}

static void writeBinTriangle(QFile& out, const Triangle& tri, const QVector3D& normal) {\
    writeBinQVector3D(out,normal);
    writeBinQVector3D(out,tri.p[0]);
    writeBinQVector3D(out,tri.p[1]);
    writeBinQVector3D(out,tri.p[2]);
    uint16_t attr = 0;
    out.write((const char*)&attr,2);
}

static void writeBinTransformedTriangle(QFile& out, const Triangle& tri, const QMatrix4x4& trans) {
    Triangle t;
    t.p[0] = trans*tri.p[0];
    t.p[1] = trans*tri.p[1];
    t.p[2] = trans*tri.p[2];
    writeBinTriangle(out, t, t.computeNormal());
}

void writeBinMeshRecurse(QFile& out, const SceneNode* node, const QMatrix4x4& transformation) {

    QMatrix4x4 newTrans = transformation;

    const TransformNode* transNode = dynamic_cast<const TransformNode*>(node);
    if(transNode != 0)
        newTrans *= transNode->matrix();

    const MeshNode* meshNode = dynamic_cast<const MeshNode*>(node);
    if(meshNode != 0) {

        const FaceNormalTriangleMesh* fntMesh = 0;
        const VertexNormalTriangleMesh* vntMesh = 0;
        fntMesh = dynamic_cast<const FaceNormalTriangleMesh*>(&meshNode->mesh());
        vntMesh = dynamic_cast<const VertexNormalTriangleMesh*>(&meshNode->mesh());

        if(fntMesh != 0)
            foreach(FaceNormalTriangle tri, fntMesh->tris())
                writeBinTransformedTriangle(out, tri, transformation);
        else if(vntMesh != 0)
            foreach(VertexNormalTriangle tri, vntMesh->tris())
                writeBinTransformedTriangle(out, tri, transformation);
        else // We should have better failure behavior here.
            throw -1;
    }

    foreach(SceneNode* sn, node->children()) {
        writeBinMeshRecurse(out, sn, newTrans);
    }
}

uint32_t countTris(const SceneNode* node) {
    const MeshNode* meshNode = dynamic_cast<const MeshNode*>(node);
    if (meshNode != 0) {
        const FaceNormalTriangleMesh* fntMesh = dynamic_cast<const FaceNormalTriangleMesh*>(&meshNode->mesh());
        const VertexNormalTriangleMesh* vntMesh = dynamic_cast<const VertexNormalTriangleMesh*>(&meshNode->mesh());
        if(fntMesh != 0)
            return fntMesh->tris().size();
        else if(vntMesh != 0)
            return vntMesh->tris().size();
        else // We should have better failure behavior here.
            throw -1;
    } else {
        uint32_t tris = 0;
        foreach(SceneNode* sn, node->children()) {
            tris += countTris(sn);
        }
        return tris;
    }
}

void BinaryStlFormatter::writeMesh(QFile &outf, const SceneNode &node) {

    // depth-first-traverse the scene tree
    //   if node is a transform node add its transform onto our transformation stack
    //   if node is a mesh node
    //     loop triangles
    //       transform each triangle, write it out

    outf.open(QIODevice::WriteOnly);

    QMatrix4x4 transformation;
    transformation.setToIdentity();

    QString name = "prototype_export";

    char nameField[80];
    memset(nameField,0,80);
    strncpy(nameField,name.toStdString().c_str(),name.size());
    outf.write(nameField, 80);

    uint32_t triCount = countTris(&node);
    outf.write((const char*)&triCount,sizeof(triCount));

    writeBinMeshRecurse(outf, &node, transformation);

    outf.flush();
    outf.close();
}

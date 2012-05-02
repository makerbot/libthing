#include "stl.h"
#include "objtree/transformnode.h"
#include "objtree/meshnode.h"

#include <QVector3D>
#include <QString>
#include <QTextStream>

static void expect(QTextStream& in, const QString& expected) {
    in.skipWhiteSpace();
    QString got = in.read(expected.length());
    if (got != expected) {
        throw ParseException(QString("Expected ") + expected);
    }
    in.skipWhiteSpace();
}

static QVector3D readVertex(QTextStream& in, const QString& expected) {
    expect(in,expected);
    float x, y, z;
    in >> x >> y >> z;
    return QVector3D(x,y,z);
}

static void readTriangle(QTextStream& in, FaceNormalTriangleMesh& mesh) {
    FaceNormalTriangle t;
    t.n = readVertex(in,"normal");
    expect(in, "outer");
    expect(in, "loop");
    for (int i = 0; i < 3; i++) {
        t.p[i] = readVertex(in,"vertex");
    }
    expect(in, "endloop");
    expect(in, "endfacet");
    mesh.addTri(t);
}

#define MAX_LINE 80

SmartMesh* AsciiStlFormatter::readMesh(QFile& inf) {
    FaceNormalTriangleMesh* mesh = new FaceNormalTriangleMesh();
    QTextStream in(&inf);
    QString lineStr = in.readLine(MAX_LINE);
    QTextStream line(&lineStr);
    line.skipWhiteSpace();
    QString word;
    line >> word;
    if (word != "solid") {
        throw ParseException("ASCII STL did not start with 'solid'");
    }
    QString name = line.readAll().trimmed();
    while (1) {
        in >> word;
        if (word == "facet") {
            readTriangle(in,*mesh);
        } else if (word == "endsolid") {
            break;
        } else {
            throw ParseException("Expected endsolid or facet");
        }
    }
    //cout << "Loaded mesh; facet count " << mesh.tris.size() << endl;
    return mesh;
}

static void writeTriangle(QTextStream& out, const Triangle& tri, const QVector3D& normal) {
    out << " facet normal " << normal.x() << " " << normal.y() << " " << normal.z() << "\n";
    out << "  outer loop"<< "\n";
    out << "    vertex " << tri.p[0].x() << " " << tri.p[0].y() << " " << tri.p[0].z() << "\n";
    out << "    vertex " << tri.p[1].x() << " " << tri.p[1].y() << " " << tri.p[1].z() << "\n";
    out << "    vertex " << tri.p[2].x() << " " << tri.p[2].y() << " " << tri.p[2].z() << "\n";
    out << "  endloop" << "\n";
    out << " endfacet" << "\n";
}

static void writeTransformedTriangle(QTextStream& out, const Triangle& tri, const QMatrix4x4& trans) {
    Triangle t;
    t.p[0] = trans*tri.p[0];
    t.p[1] = trans*tri.p[1];
    t.p[2] = trans*tri.p[2];
    writeTriangle(out, t, t.computeNormal());
}

// writes out a single, untransformed mesh
void AsciiStlFormatter::writeMesh(QFile& outf, const SmartMesh& mesh) {

    const FaceNormalTriangleMesh* fntMesh = dynamic_cast<const FaceNormalTriangleMesh*>(&mesh);
    const VertexNormalTriangleMesh* vntMesh = dynamic_cast<const VertexNormalTriangleMesh*>(&mesh);

    QTextStream out(&outf);

    out << "solid " << "solidname";

    if(fntMesh != 0)
        foreach(FaceNormalTriangle tri, fntMesh->tris())
            writeTriangle(out, tri, tri.n);
    else if(vntMesh != 0)
        foreach(VertexNormalTriangle tri, vntMesh->tris())
            writeTriangle(out, tri, tri.computeNormal());
    else // We should have better failure behavior here.
        throw -1;

    out << "endsolid " << "solidname";
}

// this could probably be turned into a loop
// this does the DFT for writeMesh(QFile, SceneNode)
void writeMeshRecurse(QTextStream& out, const SceneNode* node, const QMatrix4x4& transformation) {

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
                writeTransformedTriangle(out, tri, transformation);
        else if(vntMesh != 0)
            foreach(VertexNormalTriangle tri, vntMesh->tris())
                writeTransformedTriangle(out, tri, transformation);
        else // We should have better failure behavior here.
            throw -1;
    }

    foreach(SceneNode* sn, node->children()) {
        writeMeshRecurse(out, sn, newTrans);
    }
}

// writes out a scene tree, transforming each mesh and witing them all
// out as the same solid
void AsciiStlFormatter::writeMesh(QFile& outf, const SceneNode& node) {

    // depth-first-traverse the scene tree
    //   if node is a transform node add its transform onto our transformation stack
    //   if node is a mesh node
    //     loop triangles
    //       transform each triangle, write it out

    outf.open(QIODevice::WriteOnly | QIODevice::Text);

    QMatrix4x4 transformation;
    transformation.setToIdentity();

    QTextStream out(&outf);

    out << "solid " << "solidname" << "\n";

    writeMeshRecurse(out, &node, transformation);

    out << "endsolid " << "solidname" << "\n";
    out.flush();
}


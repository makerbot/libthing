#include "objformatter.h"

#include <QVector3D>
#include <QString>
#include <QList>
#include <QTextStream>

SmartMesh* ObjFormatter::readMesh(QFile& inf) {
    QList<QVector3D> vertexList;
    QList<QVector3D> normalList;
    const static int MAX_LINE = 4096;
    VertexNormalTriangleMesh* mesh = new VertexNormalTriangleMesh();
    QTextStream in(&inf);

    while (!in.atEnd()) {
        QString line = in.readLine(MAX_LINE);
        if (line.isEmpty() || line[0] == '#') continue;
        QStringList parts = line.simplified().split(" ");
        if (parts.size() == 0) continue;
        if (parts[0] == "v") {
            // vertex
            if (parts.size() < 4) {
                throw ParseException("Insufficient coordinates for vertex");
            }
            vertexList.append(QVector3D(parts[1].toDouble(),parts[2].toDouble(),parts[3].toDouble()));
        } else if (parts[0] == "vn") {
            // vertex normal
            if (parts.size() < 4) {
                throw ParseException("Insufficient coordinates for normal");
            }
            QVector3D normal(parts[1].toDouble(),parts[2].toDouble(),parts[3].toDouble());
            normalList.append(normal.normalized());
        } else if (parts[0] == "f") {
            // face
            bool needsNormals = false; // If set, we need to compute normals for the face
            if (parts.size() < 4) throw ParseException("Insufficient vertices for face");
            // Fan triangulation. If .obj files can contain non-convex faces, this will cause
            // some errors.
            for (int j = 0; j < parts.size()-3; j++) {
                VertexNormalTriangle t;
                for (int i = 0; i < 3; i++) {
                    // Since we're doing a fan triangulation, we need to ensure
                    // that the first vertex of every triangle is idx 0.
                    int partIdx = (i==0)?1:(1+i+j);
                    QStringList vparts = parts[partIdx].split("/");
                    int vidx = vparts[0].toInt();
                    if (vidx > vertexList.size() || vidx == 0) {
                        throw ParseException("Face requires unspecified vertex");
                    }
                    t.p[i] = vertexList[vidx-1];
                    if (vparts.size() >= 3) {
                        int nidx = vparts[2].toInt();
                        if (nidx > normalList.size() || nidx == 0) {
                            throw ParseException("Face requires unspecified normal");
                        }
                        t.n[i] = normalList[nidx-1];
                    } else if (normalList.size() >= vidx) {
                        // Implicit vertex normal
                        t.n[i] = normalList[vidx-1];
                    } else {
                        needsNormals = true;
                    }
                }
                if (needsNormals) {
                    QVector3D n = t.computeNormal();
                    for (int i = 0; i < 3; i++) {
                        t.n[i] = n;
                    }
                }
                mesh->addTri(t);
            }
        }
    }
    qDebug() << "Loaded OBJ mesh; facet count" << mesh->tris().size() << "vertices" << vertexList.size() << "normals" << normalList.size();
    return mesh;
}

void ObjFormatter::writeMesh(QFile& outf, const SmartMesh& mesh) {
    throw -1;
}
void ObjFormatter::writeMesh(QFile& outf, const SceneNode& node) {
    throw -1;
}

#ifndef MESH_H
#define MESH_H

#include <QList>
#include <QVector3D>
#include <QtOpenGL>
#include "boundingbox.h"

class MeshPrimitive {
public:
    void renderGL() {}
};

class Triangle : public MeshPrimitive {
public:
    QVector3D p[3];
    QVector3D computeNormal() const;
};

class FaceNormalTriangle : public Triangle {
public:
    QVector3D n;
};

class VertexNormalTriangle : public Triangle {
public:
    QVector3D n[3];
};

class Mesh
{
protected:
    BoundingBox m_bounds;
    GLuint m_displayList;
    QString meshName;
    QString meshFile;
public:
    Mesh() :
        m_displayList(0),
        meshName(""),
        meshFile("")
    {}
    virtual void renderGL() = 0;
    virtual ~Mesh();
    const BoundingBox& bounds() const { return m_bounds; }
    const QString& name() const { return meshName; }
    void setName(const QString& name) { meshName = name; }
    const QString& filename() const { return meshFile; }
    void setFilename(const QString& filename) { meshFile = filename; }
    void displayListRenderGL();
};

class FaceNormalTriangleMesh : public Mesh {
protected:
    QList<FaceNormalTriangle> m_tris;
public:
    virtual void renderGL();
    void addTri(const FaceNormalTriangle& tri);
    const QList<FaceNormalTriangle>& tris() const { return m_tris; }
};

class VertexNormalTriangleMesh : public Mesh {
protected:
    QList<VertexNormalTriangle> m_tris;
public:
    virtual void renderGL();
    void addTri(const VertexNormalTriangle& tri);
    const QList<VertexNormalTriangle>& tris() const { return m_tris; }
};

#endif // MESH_H

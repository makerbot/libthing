#include "mesh.h"
#include <iostream>
#include <QtOpenGL>

QVector3D Triangle::computeNormal() const {
    QVector3D d1 = p[1]-p[0];
    QVector3D d2 = p[2]-p[1];
    QVector3D n = QVector3D::crossProduct(d1,d2);
    n.normalize();
    return n;
}

inline void glAddPoint(const QVector3D& p) {
    glVertex3f(p.x(),p.y(),p.z());
}

inline void glAddNormal(const QVector3D& p) {
    glNormal3f(p.x(),p.y(),p.z());
}

void FaceNormalTriangleMesh::renderGL() {
    glBegin(GL_TRIANGLES);
    int len = m_tris.size();
    for (int i = 0; i < len; i++) {
        FaceNormalTriangle& t = m_tris[i];
        //glAddNormal(t.n);
        glAddNormal(t.computeNormal());
        glAddPoint(t.p[0]);
        glAddPoint(t.p[1]);
        glAddPoint(t.p[2]);
    }
    glEnd();
}

void VertexNormalTriangleMesh::renderGL() {
    glBegin(GL_TRIANGLES);
    int len = m_tris.size();
    for (int i = 0; i < len; i++) {
        VertexNormalTriangle& t = m_tris[i];
        glAddNormal(t.n[0]);
        glAddPoint(t.p[0]);
        glAddNormal(t.n[1]);
        glAddPoint(t.p[1]);
        glAddNormal(t.n[2]);
        glAddPoint(t.p[2]);
    }
    glEnd();
}

void FaceNormalTriangleMesh::addTri(const FaceNormalTriangle &tri)
{
    m_tris.append(tri);
    m_bounds.addPoint(tri.p[0]);
    m_bounds.addPoint(tri.p[1]);
    m_bounds.addPoint(tri.p[2]);
}

void VertexNormalTriangleMesh::addTri(const VertexNormalTriangle &tri)
{
    m_tris.append(tri);
    m_bounds.addPoint(tri.p[0]);
    m_bounds.addPoint(tri.p[1]);
    m_bounds.addPoint(tri.p[2]);
}

Mesh::~Mesh()
{
    glDeleteLists(m_displayList,1);
}

void Mesh::displayListRenderGL()
{
    if (m_displayList == 0) {
        m_displayList = glGenLists(1);
        glNewList(m_displayList,GL_COMPILE);
        renderGL();
        glEndList();
    }
    glCallList(m_displayList);
}

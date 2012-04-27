#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QVector3D>

class BoundingBox
{
private:
    QVector3D m_min, m_max;
public:
    BoundingBox();
    BoundingBox(const QVector3D min, const QVector3D max) : m_min(min), m_max(max) {}
    void addPoint(const QVector3D p);

    const QVector3D& min() const { return m_min; }
    const QVector3D& max() const { return m_max; }

    QVector3D center() const;

    BoundingBox& operator+=(const QVector3D& p) { addPoint(p); return *this; }
    BoundingBox& operator+=(const BoundingBox& b) { addPoint(b.min()); addPoint(b.max()); return *this; }

    bool operator==(const BoundingBox& b) { return min() == b.min() && max() == b.max(); }
    bool isEmpty() const;

    double xmin() const { return m_min.x(); }
    double xmax() const { return m_max.x(); }
    double ymin() const { return m_min.y(); }
    double ymax() const { return m_max.y(); }
    double zmin() const { return m_min.z(); }
    double zmax() const { return m_max.z(); }
};

#endif // BOUNDINGBOX_H

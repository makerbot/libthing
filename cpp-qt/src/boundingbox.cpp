#include "boundingbox.h"
#include <limits>
#include <algorithm>


#define FLT_MAX std::numeric_limits<float>::max()
#define FLT_MIN -FLT_MAX

BoundingBox::BoundingBox() : m_min(FLT_MAX,FLT_MAX,FLT_MAX), m_max(FLT_MIN,FLT_MIN,FLT_MIN)
{
}

void BoundingBox::addPoint(const QVector3D p)
{
    m_min.setX(std::min(p.x(),m_min.x()));
    m_min.setY(std::min(p.y(),m_min.y()));
    m_min.setZ(std::min(p.z(),m_min.z()));
    m_max.setX(std::max(p.x(),m_max.x()));
    m_max.setY(std::max(p.y(),m_max.y()));
    m_max.setZ(std::max(p.z(),m_max.z()));
}

QVector3D BoundingBox::center() const
{
    return (m_min + m_max) / 2.0;
}

bool BoundingBox::isEmpty() const
{
    return BoundingBox() == *this;
}

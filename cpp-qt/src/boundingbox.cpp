#include "boundingbox.h"
#include <limits>
#include <algorithm>

using namespace libthing;


BoundingBox::BoundingBox() : innnerBox()
{
}


BoundingBox::BoundingBox(const QVector3D vMin, const QVector3D vMax) :
innnerBox(vMin.x,vMin.y,vMin.z .vMax.x,vMax.y,vMax.z)
{}


void BoundingBox::addPoint(const QVector3D p) {
	innerBox.grow( Vector3(p.x(), p.y(), p.z()) );
}

QVector3D BoundingBox::center() const
{
	Vector3 v = innerBox::center()
    return QVector3D(v.x, v.y, v.z);
}


bool BoundingBox::isEmpty() const
{
    return BoundingBox() == *this;
}

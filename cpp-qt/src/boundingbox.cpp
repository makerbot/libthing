#include "boundingbox.h"
#include <limits>
#include <algorithm>

using namespace libthing;


BoundingBox::BoundingBox() : innerBox()
{
}


BoundingBox::BoundingBox(const QVector3D vMin, const QVector3D vMax) :
innerBox(vMin.x,vMin.y,vMin.z .vMax.x,vMax.y,vMax.z)
{}





void BoundingBox::addPoint(const QVector3D p) {
	innerBox.grow( Vector3(p.x(), p.y(), p.z()) );
}

BoundingBox& BoundingBox::operator+=(const QVector3D& p)
{
	addPoint(p);
	return *this;
}
BoundingBox& BoundingBox::operator+=(const BoundingBox& b) {
	addPoint(b.min());
	addPoint(b.max());
	return *this;
}



QVector3D BoundingBox::center() const
{
	Vector3 v = innerBox::center();
    return QVector3D(v.x, v.y, v.z);
}


bool BoundingBox::isEmpty() const
{
    return BoundingBox() == *this;
}

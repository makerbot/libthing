#include "boundingbox.h"
#include <limits>
#include <algorithm>

#include "Vector3.h"


BoundingBox::BoundingBox() :
innerBox(-MAX_SANE_BUILD,-MAX_SANE_BUILD,MAX_SANE_BUILD,
			MAX_SANE_BUILD,MAX_SANE_BUILD,MAX_SANE_BUILD)
{
//TRICKY:
// qt-style bounding box defaults to building a 'max sane'
// bounding box if unspecified. core cpp-libthing bounding boxes default
// to 0,0,0 sized bounding box.

}


BoundingBox::BoundingBox(const QVector3D vMin, const QVector3D vMax) :
    innerBox(vMin.x(),vMin.y(),vMin.z(), vMax.x(),vMax.y(),vMax.z())
{}





void BoundingBox::addPoint(const QVector3D p) {
        innerBox.grow( libthing::Vector3(p.x(), p.y(), p.z()) );
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
    libthing::Vector3 v = innerBox.center();
    return QVector3D(v.x, v.y, v.z);
}


bool BoundingBox::isEmpty() const
{
    return BoundingBox() == *this;
}

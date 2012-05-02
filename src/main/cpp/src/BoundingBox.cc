/**
   MiracleGrue - Model Generator for toolpathing. <http://www.grue.makerbot.com>
   Copyright (C) 2011 Far McKon <Far@makerbot.com>, Hugo Boyer (hugo@makerbot.com)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

*/
#include "libthing/BoundingBox.h"

using namespace libthing;


// 1e20 used as min/max
// A few kilometers larger than the build platform in 2012
#define MAX_SANE_BUILD 1e20


BoundingBox::BoundingBox(Scalar minX, Scalar minY,Scalar  minZ,Scalar maxX, Scalar maxY,Scalar maxZ ):
vMin(minX,minY,minZ),
vMax(maxX,maxY,maxZ)
{
}




/*
BoundingBox::Limits(const Limits& kat)
{
	min.x = kat.min.x;
	max.x = kat.max.x;
	min.y= kat.min.y;
	max.y = kat.max.y;
	min.z = kat.min.z;
	max.z = kat.max.z;
}
*/
void BoundingBox::grow(const Vector3 &p)
{
	vMin.x = std::min(vMin.x,p.x);
	vMin.y = std::min(vMin.y,p.y);
	vMin.z = std::min(vMin.z,p.z);

	vMax.x = std::max(vMax.x,p.x);
	vMax.y = std::max(vMax.y,p.y);
	vMax.z = std::max(vMax.z,p.z);
}

Vector3 BoundingBox::center()
{
	return vMin + vMax / 2.0;
}


// adds inflate to all sides (half of inflate in + and half inflate in - direction)
void BoundingBox::inflate(Scalar inflateX, Scalar inflateY, Scalar inflateZ)
{
	vMax.x += 0.5 * inflateX;
	vMax.y += 0.5 * inflateY;
	vMax.z += 0.5 * inflateZ;

	vMin.x += 0.5 * -inflateX;
	vMin.y += 0.5 * -inflateY;
	vMin.z += 0.5 * -inflateZ;
}

// grows the limits to contain points that rotate along
// the XY center point and Z axis
void BoundingBox::tubularZ()
{
	Vector3 c = center();
	Scalar dx = 0.5 * (vMax.x-vMin.x);
	Scalar dy = 0.5 * (vMax.y- vMin.y);

	Scalar radius = sqrt(dx*dx + dy*dy);

	Vector3 north = c;
	north.y += radius;

	Vector3 south = c;
	south.y -= radius;

	Vector3 east = c;
	east.x += radius;

	Vector3 west = c;
	west.x -= radius;

	grow(north);
	grow(south);
	grow(east);
	grow(west);
}

Vector3 BoundingBox::center() const
{
	Vector3 c(0.5 * (vMin.x + vMax.x), 0.5 * (vMin.y+ vMax.y), 0.5 *(vMin.z + vMax.z) );
	return c;
}

Scalar BoundingBox::deltaX() const
{
	return (vMax.x - vMin.x);
}

Scalar BoundingBox::deltaY() const
{
	return (vMax.y - vMin.y);
}

BoundingBox BoundingBox::centeredLimits() const
{
	BoundingBox out;
	out.vMax.x = 0.5 * deltaX();
	out.vMin.x = -out.vMax.x;
	out.vMax.y = 0.5 * deltaY();
	out.vMin.y = -out.vMax.y;
	out.vMin.z = vMin.z;
	out.vMax.z = vMax.z;
	return out;
}

bool BoundingBox::isEmpty() const
{
    return BoundingBox() == *this;
}

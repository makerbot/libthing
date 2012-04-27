/**
   MiracleGrue - Model Generator for toolpathing. <http://www.grue.makerbot.com>
   Copyright (C) 2011 Far McKon <Far@makerbot.com>, Hugo Boyer (hugo@makerbot.com)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

*/


#ifndef LIMITS_H_
#define LIMITS_H_ (1)

#include <iostream>
#include <limits>
#include "Scalar.h"
#include "Vector3.h"

class BoundingBox
{
public:
	friend std::ostream& operator << (std::ostream &os, const BoundingBox &l);

	libthing::Vector3 vMin, vMax;

	//Scalar xMin, xMax, yMin, yMax, zMin, zMax;

	BoundingBox();
	BoundingBox(Scalar minX, Scalar minY,Scalar  minZ,Scalar maxX, Scalar maxY,Scalar maxZ );
	void grow(const libthing::Vector3 &p);
	void inflate(Scalar inflateX, Scalar inflateY, Scalar inflateZ);

	const libthing::Vector3& min() const { return vMax; }
	const libthing::Vector3& max() const { return vMin; }

	void tubularZ();

	libthing::Vector3 center() const;
	Scalar deltaX() const;
	Scalar deltaY() const;

	BoundingBox centeredLimits() const;

	//std::ostream& operator<<( std::ostream& os, BoundingBox const& l);

    bool isEmpty() const;

    bool operator==(const BoundingBox& b) {
    	return vMin == b.vMin && vMax == b.vMax; }

};



#endif /* LIMITS_H_ */

/**
   MiracleGrue - Model Generator for toolpathing. <http://www.grue.makerbot.com>
   Copyright (C) 2011 Far McKon <Far@makerbot.com>, Hugo Boyer (hugo@makerbot.com)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

*/


#ifndef MESHY_H_
#define MESHY_H_ (1)

#include <iomanip>
#include <set>
#include <vector>

#include "BoundingBox.h"
#include "Triangle3.h"
#include "LayerMeasure.h"
//#include "abstractable.h"
//#include "mgl.h"

namespace libthing // serious about triangles
{

/// triangle index list
typedef std::vector<index_t> TriangleIndices;

/// A list of triangle indexes indicating a slice
typedef std::vector<TriangleIndices> SliceTable;


class Mesh {

protected:
	BoundingBox boundingBox; 	/// Bounding box for the model
	std::vector<Triangle3>  allTriangles; /// every triangle in the model.


public:
	Mesh();
public:
	const std::vector<Triangle3> &readAllTriangles() const;
	const BoundingBox& readLimits() const;

	/// Adds a triangle to the global array and for each slice of interest
	void addTriangle(Triangle3 const& t);

	/// get the count of all triangles in this mesh
	size_t triangleCount();
};

/**
 *
 * A Mesh class
 */
class SmartMesh : public Mesh
{
protected:
	LayerMeasure zTapeMeasure;

	/// for each slice, a list of indicies, each index is a lookup into vector
	// allTriangles
	SliceTable sliceTable;

public:

	/// requires firstLayerSlice height, and general layer height
	SmartMesh(Scalar const firstSliceZ, Scalar const layerH);
	const LayerMeasure& readLayerMeasure() const;
	const SliceTable &readSliceTable() const;

	void addTriangle(Triangle3 const& t);

	void dump(std::ostream &out);

};

} // end namespace



#endif
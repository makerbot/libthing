
#include "libthing/Mesh.h"

#include <iostream>
#include <stdint.h>
#include <cstring>

#include <fstream>

#include <cstdlib>


using namespace libthing;
using namespace std;

//#include "log.h"

Mesh::Mesh(): boundingBox()
{
	// this space intentionally left blank
}


const std::vector<Triangle3>& Mesh::readAllTriangles() const
{
	return allTriangles;
}

const BoundingBox& Mesh::readLimits() const
{
	return boundingBox;
}


void Mesh::addTriangle(Triangle3 const& t)
{
	allTriangles.push_back(t);
	boundingBox.grow(t[0]);
	boundingBox.grow(t[1]);
	boundingBox.grow(t[2]);

}

void FaceNormalTriangleMesh::addTriangle(Triangle3 const& t)
{
	throw -1;
}

void FaceNormalTriangleMesh::addTriangle(FaceNormTriangle3 const& t)
{
	allFaceNormTriangles.push_back(t);
	boundingBox.grow(t[0]);
	boundingBox.grow(t[1]);
	boundingBox.grow(t[2]);

}

void VertexNormalTriangleMesh::addTriangle(Triangle3 const& t)
{
	throw -1;
}

void VertexNormalTriangleMesh::addTriangle(VertexNormTriangle3 const& t)
{
	allVertexNormTriangles.push_back(t);
	boundingBox.grow(t[0]);
	boundingBox.grow(t[1]);
	boundingBox.grow(t[2]);

}



size_t Mesh::triangleCount() {
	return allTriangles.size();
   //Log::often() << "all triangle count" << allTriangles.size();
}




	/// requires firstLayerSlice height, and general layer height
SmartMesh::SmartMesh(Scalar const firstSliceZ, Scalar const layerH)
		:Mesh(), zTapeMeasure(firstSliceZ, layerH)
{
	// this space intentionally left blank
}


const LayerMeasure& SmartMesh::readLayerMeasure() const
{
	return zTapeMeasure;
}

const SliceTable& SmartMesh::readSliceTable() const
{
	return sliceTable;
}

//
// Adds a triangle to the global array and for each slice of interest
//
void SmartMesh::addTriangle(Triangle3 const& t)
{
	Vector3 a, b, c;
	t.zSort(a,b,c);

	unsigned int minSliceIndex = this->zTapeMeasure.zToLayerAbove(a.z);
	if(minSliceIndex > 0)
		minSliceIndex-- ;

	unsigned int maxSliceIndex = this->zTapeMeasure.zToLayerAbove(c.z);
	if (maxSliceIndex - minSliceIndex > 1)
		maxSliceIndex-- ;

//	Log::never()  << "Min scalar = [" <<  a.z << ", "<< c.z << "]"<< std::endl;
//	Log::never()  << "Min max index = [" <<  minSliceIndex << ", "<< maxSliceIndex << "]"<< std::endl;
//	Log::never()  << "Max index =" <<  maxSliceIndex << std::endl;

	unsigned int currentSliceCount = sliceTable.size();
	if (maxSliceIndex >= currentSliceCount)
	{
		unsigned int newSize = maxSliceIndex+1;
		sliceTable.resize(newSize); // make room for potentially new slices
//		Log::often() << "- new slice count: " << sliceTable.size() << std::endl;
	}

	allTriangles.push_back(t);

	size_t newTriangleId = allTriangles.size() -1;

	//Log::often()  << "adding triangle " << newTriangleId << " to layer " << minSliceIndex  << " to " << maxSliceIndex << std::endl;

	/// TRICKY: To err on the side of success, we "OVER ADD LAYERS"
	/// and add layer info rounding to the layer above.

	for (size_t i= minSliceIndex; i<= maxSliceIndex; i++)
	{
		TriangleIndices &trianglesForSlice = sliceTable[i];
		trianglesForSlice.push_back(newTriangleId);
		//Log::often()  << "   !adding triangle " << newTriangleId << " to layer " << i  << " (size = " << trianglesForSlice.size() << ")" << std::endl;
	}

	boundingBox.grow(t[0]);
	boundingBox.grow(t[1]);
	boundingBox.grow(t[2]);

	//Log::often() << "Min max index = [" <<  minSliceIndex << ", "<< maxSliceIndex << "]"<< std::endl;
	//Log::often() << "Max index =" <<  maxSliceIndex << std::endl;


}



void SmartMesh::dump(std::ostream &out)
{
	//out << "dumping " << this << std::endl;
	//out << "Nb of triangles: " << allTriangles.size() << std::endl;
	size_t sliceCount = sliceTable.size();

	//out << "triangles per slice: (" << sliceCount << " slices)" << std::endl;
	for (size_t i= 0; i< sliceCount; i++)
	{
		TriangleIndices &trianglesForSlice = sliceTable[i];
		//trianglesForSlice.push_back(newTriangleId);
		//out << "  slice " << i << " size: " << trianglesForSlice.size() << std::endl;
        //Log::often() << "adding triangle " << newTriangleId << " to layer " << i << std::endl;
	}
}




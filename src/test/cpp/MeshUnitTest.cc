#include <fstream>

#include <cstdlib>

#include <cppunit/config/SourcePrefix.h>

#include "UnitTestUtils.h"
#include "MeshUnitTest.h"


#include "libthing/Scalar.h"
#include "libthing/Vector3.h"
#include "libthing/Triangle3.h"
#include "libthing/Mesh.h"

#include "libthing/PlatformTools.h"
//#include "libthing/Exception.h"
//#include "libthing/LineSegment2.h"

using namespace std;
using namespace libthing;


string testCaseInputsDir("test_cases/MeshTestCase/inputs/");


CPPUNIT_TEST_SUITE_REGISTRATION( MeshTestCase );
#include <float.h>

string outputDir("outputs/test_cases/MeshTestCase/");

void MeshTestCase::setUp()
{
	std::cout<< " Starting:" <<__FUNCTION__ << endl;
	OperatingSystem computer;
	mkDebugPath(outputDir.c_str());
}



void MeshTestCase::testMesh() {

	Vector3 a(0,0,0),b (0,0,1), c(0,1,0),d(0,1,1),
			e(1,0,0), f(1,0,1), g(1,1,0),h(1,1,1);
	Mesh* m1 = new Mesh();

	printf("## test Mesh default bounding box is 0 sized\n");
	const BoundingBox* bb = &m1->readLimits();
	CPPUNIT_ASSERT( bb->vMin == a);
	CPPUNIT_ASSERT( bb->vMax == a);


	printf("## test adding a single triangle to Mesh\n");
	Triangle3 tA(a,b,c);
	m1->addTriangle(tA);
	CPPUNIT_ASSERT( m1->triangleCount() == (size_t)1);

	const BoundingBox* bb2 = &m1->readLimits();
	CPPUNIT_ASSERT( bb2->vMin == a);
	CPPUNIT_ASSERT( bb2->vMax == d);


	printf("## test adding 2nd triangle to Mesh\n");
	Triangle3 tB(b,c,e);
	m1->addTriangle(tB);
	CPPUNIT_ASSERT( m1->triangleCount() == (size_t)2);

	const BoundingBox* bb3 = &m1->readLimits();
	//printf("Max: %f, %f, %f\n", bb3->vMax.x, bb3->vMax.y, bb3->vMax.z);
	//printf("Min: %f, %f, %f\n", bb3->vMin.x, bb3->vMin.y, bb3->vMin.z);
	CPPUNIT_ASSERT( bb3->vMin == a);
	CPPUNIT_ASSERT( bb3->vMax == h);

}



void MeshTestCase::testSmartMesh() {

	Vector3 a(0,0,0),b (0,0,1), c(0,1,0),d(0,1,1),
			e(1,0,0), f(1,0,1), g(1,1,0),h(1,1,1);
	Scalar firstSliceZ = 0.0, sliceHeight=0.2;

	SmartMesh* m1 = new SmartMesh(firstSliceZ, sliceHeight);

	printf("## test SmartMesh default bounding box is 0 sized\n");
	const BoundingBox* bb = &m1->readLimits();
	CPPUNIT_ASSERT( bb->vMin == a);
	CPPUNIT_ASSERT( bb->vMax == a);

	printf("## test adding a single triangle to SmartMesh\n");
	Triangle3 tA(a,b,c);
	m1->addTriangle(tA);
	CPPUNIT_ASSERT( m1->triangleCount() == (size_t)1);

	const BoundingBox* bb2 = &m1->readLimits();
	CPPUNIT_ASSERT( bb2->vMin == a);
	CPPUNIT_ASSERT( bb2->vMax == d);

	const SliceTable* sliceTable1 = &m1->readSliceTable();
	printf("sliceTableSize %d", (unsigned int)sliceTable1->size());

	/// TRICKY: To err on the side of success, we "OVER-ADD LAYERS"
	/// and add layer info rounding to the layer above.
	CPPUNIT_ASSERT( sliceTable1->size() == (1.0/sliceHeight) + 1);

	printf("## test adding 2nd triangle to SmartMesh\n");
	Triangle3 tB(b,c,e);
	m1->addTriangle(tB);
	CPPUNIT_ASSERT( m1->triangleCount() == (size_t)2);

	const BoundingBox* bb3 = &m1->readLimits();
	//printf("Max: %f, %f, %f\n", bb3->vMax.x, bb3->vMax.y, bb3->vMax.z);
	//printf("Min: %f, %f, %f\n", bb3->vMin.x, bb3->vMin.y, bb3->vMin.z);
	CPPUNIT_ASSERT( bb3->vMin == a);
	CPPUNIT_ASSERT( bb3->vMax == h);


}


//void  MeshTestCase::testScalarBasics(){
//	/// TODO: test scalar min/max/etc
//}
//
//void  MeshTestCase::testScalarMaths(){
//	/** basic math (assuming it works)*/
//
//	/** complex math **/
//
//	/** helper functions **/
//	Scalar s1 = 0, s2 = 0;
//	CPPUNIT_ASSERT( tequals(s1, s1, 0.2) == true);
//	CPPUNIT_ASSERT( tequals(s1, s1, 1) == true);
//	s1 = 10, s2 = 10.1;
//	CPPUNIT_ASSERT( tequals(s1, s2, 0.05) == false);
//	CPPUNIT_ASSERT( tequals(s1, s2, 1) == true);
//	s1 = 0, s2 = SCALAR_EPSILON;
//	CPPUNIT_ASSERT( tequals(s1, s2, 0) == false);
//	s1 = 0, s2 = 0;
//	Scalar t1 = SCALAR_EPSILON;
//	CPPUNIT_ASSERT( tequals(s1, s2, t1) == true);
//
//	// Note the behavior of tequals vs. SCALAR_EPSILON
//	s1 = 0, s2 = SCALAR_EPSILON;
//	CPPUNIT_ASSERT( tequals(s1, s2, 0) == false);
//
//	// Note the behavior of tequals vs. SCALAR_EPSILON
//	s1 = 0, s2 = SCALAR_EPSILON;
//	t1 = SCALAR_EPSILON;
//	CPPUNIT_ASSERT( tequals(s1, s2, t1) == false);
//
//}
//
//void  MeshTestCase::testVector2Basics()
//{
//	Vector2 foo;
//	CPPUNIT_ASSERT( foo.x == 0);
//	CPPUNIT_ASSERT( foo.y == 0);
//
//	Vector2 bar(10,100);
//	CPPUNIT_ASSERT( bar[0] == 10);
//	CPPUNIT_ASSERT( bar[1] == 100);
//	CPPUNIT_ASSERT_THROW(bar[2], Exception);
//
//	bar[0] = 20; bar[1] = 200;
//	CPPUNIT_ASSERT( bar[0] == 20);
//	CPPUNIT_ASSERT( bar[1] == 200);
//	CPPUNIT_ASSERT_THROW(bar[2] = 2000, Exception);
//}
//
//
//
//void MeshTestCase::testLineSegment2dBasics()
//{
//	LineSegment2 seg1;
//	CPPUNIT_ASSERT( seg1.a.x == 0);
//	CPPUNIT_ASSERT( seg1.a.y == 0);
//	CPPUNIT_ASSERT( seg1.b.x == 0);
//	CPPUNIT_ASSERT( seg1.b.y == 0);
//
//	Vector2 v1(10,20), v2(100,200);
//	LineSegment2 seg2(v1, v2);
//	CPPUNIT_ASSERT( seg2.a.x == 10);
//	CPPUNIT_ASSERT( seg2.a.y == 20);
//	CPPUNIT_ASSERT( seg2.b.x == 100);
//	CPPUNIT_ASSERT( seg2.b.y == 200);
//
//	LineSegment2 seg3 = seg2;
//	CPPUNIT_ASSERT( (void*)&seg2 != (void*)&seg3);
//	CPPUNIT_ASSERT( (void*)&seg2.a != (void*)&seg3.a);
//	CPPUNIT_ASSERT( (void*)&seg2.a.x != (void*)&seg3.a.x);
//
//}

//void MeshTestCase::testVector3Basics()
//{
//	Vector3 foo;
//	CPPUNIT_ASSERT( foo.x == 0);
//	CPPUNIT_ASSERT( foo.y == 0);
//	CPPUNIT_ASSERT( foo.z == 0);
//	Vector3 bar = foo;
//	CPPUNIT_ASSERT( (void*)&bar != (void*)&foo);
//	CPPUNIT_ASSERT( (void*)&bar.x != (void*)&foo.x);
//
//	Vector3 baz(10, 40, 50 );
//	CPPUNIT_ASSERT( baz[0] == 10);
//	CPPUNIT_ASSERT( baz[1] == 40);
//	CPPUNIT_ASSERT( baz[2] == 50);
//	CPPUNIT_ASSERT_THROW(baz[3] = 20, Exception);
//
//	baz[0] = 100; baz[1] = 400, baz[2] = 500;
//	CPPUNIT_ASSERT( baz.x == 100);
//	CPPUNIT_ASSERT( baz.y == 400);
//	CPPUNIT_ASSERT( baz.z == 500);
//
//	//verify Vector3.tequals
//	Vector3 v0(0,0,0);
//	Vector3 v1(0,0,SCALAR_EPSILON*2);
//	CPPUNIT_ASSERT( v0.tequals(v1,SCALAR_EPSILON ) == false );
//	Vector3 v2(0,0,0);
//	Vector3 v3(0,0,SCALAR_EPSILON/2);
//	CPPUNIT_ASSERT( v2.tequals(v3,SCALAR_EPSILON ) == true );
//
//}

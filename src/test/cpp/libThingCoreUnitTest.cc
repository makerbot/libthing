#include <fstream>

#include <cstdlib>

#include <cppunit/config/SourcePrefix.h>

#include "UnitTestUtils.h"
#include "libThingCoreUnitTest.h"


#include "libthing/PlatformTools.h"
#include "libthing/Exception.h"
#include "libthing/Scalar.h"
#include "libthing/Vector2.h"
#include "libthing/LineSegment2.h"
#include "libthing/Vector3.h"

using namespace std;
using namespace libthing;


string testCaseInputsDir("test_cases/libThingTestCase/inputs/");


CPPUNIT_TEST_SUITE_REGISTRATION( LibThingTestCase );
#include <float.h>

string outputDir("outputs/test_cases/libThingTestCase/");

void LibThingTestCase::setUp()
{
	std::cout<< " Starting:" <<__FUNCTION__ << endl;
	OperatingSystem computer;
	mkDebugPath(outputDir.c_str());
}

void throwsException(){
	Exception e("Test");
	throw e;
}

void LibThingTestCase::testExceptions() {
	CPPUNIT_ASSERT_THROW(throwsException(), Exception);
}


void  LibThingTestCase::testScalarBasics(){
	/// TODO: test scalar min/max/etc
}

void  LibThingTestCase::testScalarMaths(){
	/** basic math (assuming it works)*/

	/** complex math **/

	/** helper functions **/
	Scalar s1 = 0, s2 = 0;
	CPPUNIT_ASSERT( tequals(s1, s1, 0.2) == true);
	CPPUNIT_ASSERT( tequals(s1, s1, 1) == true);
	s1 = 10, s2 = 10.1;
	CPPUNIT_ASSERT( tequals(s1, s2, 0.05) == false);
	CPPUNIT_ASSERT( tequals(s1, s2, 1) == true);
	s1 = 0, s2 = SCALAR_EPSILON;
	CPPUNIT_ASSERT( tequals(s1, s2, 0) == false);
	s1 = 0, s2 = 0;
	Scalar t1 = SCALAR_EPSILON;
	CPPUNIT_ASSERT( tequals(s1, s2, t1) == true);

	// Note the behavior of tequals vs. SCALAR_EPSILON
	s1 = 0, s2 = SCALAR_EPSILON;
	CPPUNIT_ASSERT( tequals(s1, s2, 0) == false);

	// Note the behavior of tequals vs. SCALAR_EPSILON
	s1 = 0, s2 = SCALAR_EPSILON;
	t1 = SCALAR_EPSILON;
	CPPUNIT_ASSERT( tequals(s1, s2, t1) == false);

}

void  LibThingTestCase::testVector2Basics()
{
	Vector2 foo;
	CPPUNIT_ASSERT( foo.x == 0);
	CPPUNIT_ASSERT( foo.y == 0);

	Vector2 bar(10,100);
	CPPUNIT_ASSERT( bar[0] == 10);
	CPPUNIT_ASSERT( bar[1] == 100);
	CPPUNIT_ASSERT_THROW(bar[2], Exception);

	bar[0] = 20; bar[1] = 200;
	CPPUNIT_ASSERT( bar[0] == 20);
	CPPUNIT_ASSERT( bar[1] == 200);
	CPPUNIT_ASSERT_THROW(bar[2] = 2000, Exception);
}



void LibThingTestCase::testLineSegment2dBasics()
{
	LineSegment2 seg1;
	CPPUNIT_ASSERT( seg1.a.x == 0);
	CPPUNIT_ASSERT( seg1.a.y == 0);
	CPPUNIT_ASSERT( seg1.b.x == 0);
	CPPUNIT_ASSERT( seg1.b.y == 0);

	Vector2 v1(10,20), v2(100,200);
	LineSegment2 seg2(v1, v2);
	CPPUNIT_ASSERT( seg2.a.x == 10);
	CPPUNIT_ASSERT( seg2.a.y == 20);
	CPPUNIT_ASSERT( seg2.b.x == 100);
	CPPUNIT_ASSERT( seg2.b.y == 200);

	LineSegment2 seg3 = seg2;
	CPPUNIT_ASSERT( (void*)&seg2 != (void*)&seg3);
	CPPUNIT_ASSERT( (void*)&seg2.a != (void*)&seg3.a);
	CPPUNIT_ASSERT( (void*)&seg2.a.x != (void*)&seg3.a.x);

}

void LibThingTestCase::testVector3Basics()
{
	Vector3 foo;
	CPPUNIT_ASSERT( foo.x == 0);
	CPPUNIT_ASSERT( foo.y == 0);
	CPPUNIT_ASSERT( foo.z == 0);
	Vector3 bar = foo;
	CPPUNIT_ASSERT( (void*)&bar != (void*)&foo);
	CPPUNIT_ASSERT( (void*)&bar.x != (void*)&foo.x);

	Vector3 baz(10, 40, 50 );
	CPPUNIT_ASSERT( baz[0] == 10);
	CPPUNIT_ASSERT( baz[1] == 40);
	CPPUNIT_ASSERT( baz[2] == 50);
	CPPUNIT_ASSERT_THROW(baz[3] = 20, Exception);

	baz[0] = 100; baz[1] = 400, baz[2] = 500;
	CPPUNIT_ASSERT( baz.x == 100);
	CPPUNIT_ASSERT( baz.y == 400);
	CPPUNIT_ASSERT( baz.z == 500);

	//verify Vector3.tequals
	Vector3 v0(0,0,0);
	Vector3 v1(0,0,SCALAR_EPSILON*2);
	CPPUNIT_ASSERT( v0.tequals(v1,SCALAR_EPSILON ) == false );
	Vector3 v2(0,0,0);
	Vector3 v3(0,0,SCALAR_EPSILON/2);
	CPPUNIT_ASSERT( v2.tequals(v3,SCALAR_EPSILON ) == true );

}

#ifndef MGL_CORE_TEST_CASE_H_
#define GCODER_TEST_CASE_H_

#include <cppunit/extensions/HelperMacros.h>

/*
 * A test case that is designed to produce
 * example errors and failures
 *
 */
class LibThingTestCase : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( LibThingTestCase );

  // test base exception class
  CPPUNIT_TEST( testExceptions );

  // test base scalar value
  CPPUNIT_TEST( testScalarBasics );
  CPPUNIT_TEST( testScalarMaths );

  CPPUNIT_TEST( testVector2Basics );

  CPPUNIT_TEST(testLineSegment2dBasics);
  CPPUNIT_TEST( testVector3Basics );


  CPPUNIT_TEST_SUITE_END();



public:
  void setUp();

protected:
 void testExceptions();

 void  testScalarBasics();
 void  testScalarMaths();

 void  testVector2Basics();
 void  Vector2Maths();

 void testLineSegment2dBasics();

 void testVector3Basics();

 void testTriangle3Maths();

 void testTequalsPolygon();
 void testTequalsPolygons();


 void testMeshyLoads();

 void testSliceDataConstructorDestructor();

};


#endif /*MGL_CORE_TEST_CASE_H_*/


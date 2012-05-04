
#ifndef MODEL_READER_TEST_CASE_H
#define MODEL_READER_TEST_CASE_H (1)

#include <cppunit/extensions/HelperMacros.h>

/*
 * A test case that is designed to produce
 * example errors and failures
 *
 */

class ModelReaderTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( ModelReaderTestCase );

  CPPUNIT_TEST( testAsciiStlFormatterCycleNull );
  CPPUNIT_TEST( testAsciiStlFormatterCycleMin );
  CPPUNIT_TEST( testAsciiStlFormatterCycle );

  CPPUNIT_TEST( testBinaryStlFormatterCycle );

  CPPUNIT_TEST( testObjFormatterCycle );
  CPPUNIT_TEST_SUITE_END();


public:
  void setUp();

protected:

  void testAsciiStlFormatterCycleNull();
  void testAsciiStlFormatterCycleMin();
  void testAsciiStlFormatterCycle();

  void testBinaryStlFormatterCycle();
  void testObjFormatterCycle();
};


#endif

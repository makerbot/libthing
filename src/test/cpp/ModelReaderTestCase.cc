#include <sys/stat.h>
#include <time.h>
#include <fstream>
#include <algorithm> // find
#include <iterator>  // distance
#include <iomanip>
#include <limits>
#include <set>


#define  CPPUNIT_ENABLE_NAKED_ASSERT 1

#include <cppunit/config/SourcePrefix.h>
#include "ModelReaderTestCase.h"

#include "UnitTestUtils.h"

#include "libthing/StlFormatters.h"
#include "libthing/PlatformTools.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ModelReaderTestCase );

using namespace std;
using namespace libthing;


string outputsDir("outputs/test_cases/modelReaderTestCase/");
string inputsDir("test_cases/modelReaderTestCase/");

/*
CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, 1.1, 0.05 );
CPPUNIT_ASSERT_EQUAL( 12, 12 );
CPPUNIT_ASSERT( 12L == 12L );
*/
void ModelReaderTestCase::setUp()
{
	//this space intentionally left blank
}

void ModelReaderTestCase::testAsciiStlFormatterCycle()
{
	unsigned int t0,t1;
	string target = inputsDir + "3D_Knot.stl";
	string drop = outputsDir + "3D_Knot.stl";
	string drop2 = outputsDir + "3D_Knot_v2.stl";

	AsciiStlFormatter AsciiStlFormatter;

	cout << "Reading test file:"  << target << endl;

	std::ifstream stream;
	stream.open(target.c_str());
	t0=clock();
	Mesh* mesh1 = AsciiStlFormatter.readMesh(stream);
	t1=clock()-t0;
	stream.close();
	//mesh->dump(cout);
	cout << "Read: " << target <<" in seconds: " << t1 << endl;

	cout << "Writing test file:"  << drop << endl;
	std::ofstream outStream;
	outStream.open(drop.c_str());
	t0=clock();
	AsciiStlFormatter.writeMesh(outStream,*mesh1);
	t1=clock()-t0;
	outStream.close();
	cout << "Wrote: " << drop <<" in seconds: " << t1 << endl;


	cout << "Reload test, reloading file: "  << drop  << endl;
	stream.open(drop.c_str());
	t0=clock();
	Mesh* mesh2 = AsciiStlFormatter.readMesh(stream);
	stream.close();
	t1=clock()-t0;
	cout << "Re-Read: " << target <<" in seconds: " << t1 << endl;

	//CPPUNIT_ASSERT(*mesh1 == *mesh2);

	cout << "Re-Writing test file: "  << drop2 << endl;
	outStream.open(drop2.c_str());
	AsciiStlFormatter.writeMesh(outStream,*mesh2);
	outStream.close();
	unsigned int t2=clock()-t1;
	cout << "Re-Wrote: " << drop2 <<" in seconds: " << t2 << endl;

}


void ModelReaderTestCase::testAsciiStlFormatterCycleNull()
{
	unsigned int t0,t1;
	string target = inputsDir + "Null.stl";
	string drop = outputsDir + "Null.stl";
	string drop2 = outputsDir + "Null_v2.stl";


	AsciiStlFormatter AsciiStlFormatter;

	cout << "Reading test file:"  << target << endl;

	std::ifstream stream;
	stream.open(target.c_str());
	t0=clock();
	Mesh* mesh1 = AsciiStlFormatter.readMesh(stream);
	stream.close();
	t1=clock()-t0;
	//dump(cout);
	//->dump(cout);
	cout << "Read: " << target <<" in seconds: " << t1 << endl;

	cout << "Writing test file:"  << drop << endl;

	std::ofstream outStream;
	outStream.open(drop.c_str());
	AsciiStlFormatter.writeMesh(outStream,*mesh1);
	//mesh3.writeStlFile( drop.c_str());
	outStream.close();
	unsigned int t2=clock()-t1;
	cout << "Wrote: " << drop <<" in seconds: " << t2 << endl;

	cout << "Reload test, reloading file: "  << drop << endl;
	stream.open(drop.c_str());
	t0=clock();
	Mesh* mesh2 = AsciiStlFormatter.readMesh(stream);
	stream.close();
	t1=clock()-t0;
	cout << "Re-Read: " << target <<" in seconds: " << t1 << endl;

	//CPPUNIT_ASSERT(*mesh1 == *mesh2);

	cout << "Re-Writing test file: "  << drop2 << endl;
	outStream.open(drop2.c_str());
	AsciiStlFormatter.writeMesh(outStream,*mesh2);
	outStream.close();
	t2=clock()-t1;
	cout << "Re-Wrote: " << drop2 <<" in seconds: " << t2 << endl;
}

void ModelReaderTestCase::testAsciiStlFormatterCycleMin()
{
	unsigned int t0,t1;
	string target = inputsDir + "OneTriangle.stl";
	string drop = outputsDir + "OneTriangle.stl";
	string drop2 = outputsDir + "OneTriangle_v2.stl";

	AsciiStlFormatter AsciiStlFormatter;

	cout << "Reading test file:"  << target << endl;

	std::ifstream stream;
	stream.open(target.c_str());
	t0=clock();
	Mesh* mesh1 = AsciiStlFormatter.readMesh(stream);
	stream.close();
	t1=clock()-t0;
	//dump(cout);
	//->dump(cout);
	cout << "Read: " << target <<" in seconds: " << t1 << endl;

	cout << "Writing test file:"  << drop << endl;

	std::ofstream outStream;
	outStream.open(drop.c_str());
	AsciiStlFormatter.writeMesh(outStream,*mesh1);
	//mesh3.writeStlFile( drop.c_str());
	outStream.close();
	unsigned int t2=clock()-t1;
	cout << "Wrote: " << drop <<" in seconds: " << t2 << endl;

	cout << "Reload test, reloading file: "  << drop << endl;
	stream.open(drop.c_str());
	t0=clock();
	Mesh* mesh2 = AsciiStlFormatter.readMesh(stream);
	stream.close();
	t1=clock()-t0;
	cout << "Re-Read: " << target <<" in seconds: " << t1 << endl;
}





void ModelReaderTestCase::testBinaryStlFormatterCycle()
{
	unsigned int t0,t1;
	string target = inputsDir + "teapot.bin.stl";
	string drop = outputsDir + "teapot.bin.stl";
//	string drop2 = outputsDir + "3D_Knot_v2.stl";

	BinaryStlFormatter BinaryStlFormatter;

	cout << "Reading test file:"  << target << endl;

	std::ifstream stream;
	stream.open(target.c_str());
	t0=clock();
	Mesh* mesh1 = BinaryStlFormatter.readMesh(stream);
	t1=clock()-t0;
	stream.close();
	//mesh->dump(cout);
	cout << "Read: " << target <<" in seconds: " << t1 << endl;
//
	cout << "Writing test file:"  << drop << endl;
	std::ofstream outStream;
	outStream.open(drop.c_str());
	t0=clock();
	BinaryStlFormatter.writeMesh(outStream,*mesh1);
//	t1=clock()-t0;
//	outStream.close();
//	cout << "Wrote: " << drop <<" in seconds: " << t1 << endl;


//	cout << "Reload test, reloading file: "  << drop  << endl;
//	stream.open(drop.c_str());
//	t0=clock();
//	Mesh* mesh2 = BinaryStlFormatter.readMesh(stream);
//	stream.close();
//	t1=clock()-t0;
//	cout << "Re-Read: " << target <<" in seconds: " << t1 << endl;

	//CPPUNIT_ASSERT(*mesh1 == *mesh2);

//	cout << "Re-Writing test file: "  << drop2 << endl;
//	outStream.open(drop2.c_str());
//	BinaryStlFormatter.writeMesh(outStream,*mesh2);
//	outStream.close();
//	unsigned int t2=clock()-t1;
//	cout << "Re-Wrote: " << drop2 <<" in seconds: " << t2 << endl;

}




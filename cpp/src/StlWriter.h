#ifndef _STL_WRITER_H_
#define STL_WRITER_H_ (1)

#include <iostream>
#include <stdint.h>
#include <cstring>

#include <iomanip>
#include <set>
#include <fstream>

#include "Triangle3.h"

namespace libthing {
// simple class that writes
// a simple text file STL
class StlWriter
{

//solid Default
//  facet normal 1.435159e-01 2.351864e-02 9.893685e-01
//    outer loop
//      vertex -7.388980e-02 -2.377973e+01 6.062650e+01
//      vertex -1.193778e-01 -2.400027e+01 6.063834e+01
//      vertex -4.402440e-06 -2.490700e+01 6.064258e+01
//    endloop
//  endfacet
//endsolid Default

	std::ofstream out;
	std::string solidName;

public:
	void open(char const *fileName, char const *solid="Default");
	void writeTriangle(libthing::Triangle3 const& t);
	void close();

};

}; //end namespace

#endif /*STL_WRITER_H_*/

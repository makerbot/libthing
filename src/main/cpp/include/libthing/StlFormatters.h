#ifndef STL_FORMATTER_H
#define STL_FORMATTER_H (1)

#include "libthing/Formatter.h"
#include "libthing/Mesh.h"

#include <fstream>

namespace libthing {

class AsciiStlFormatter : public Formatter {

public:
  libthing::Mesh* readMesh(std::istream& inFh);
  void writeMesh(std::ostream& outFh, const libthing::Mesh& mesh);
};

class BinaryStlFormatter : public Formatter {

public:
	  libthing::Mesh* readMesh(std::istream& inFh);
	  void writeMesh(std::ostream& outFh, const libthing::Mesh& mesh);
};

} //end namespace libthing
#endif // STL_FORMATTER_H

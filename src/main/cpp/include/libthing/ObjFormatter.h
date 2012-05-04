#ifndef OBJFORMATTER_H
#define OBJFORMATTER_H

#include "Formatter.h"

namespace libthing {

class ObjFormatter : public Formatter
{
public:
        libthing::Mesh* readMesh(std::istream& inFh);
        void writeMesh(std::ostream& outFh, const libthing::Mesh& mesh);
};

} //end namespace libthing
#endif // OBJFORMATTER_H

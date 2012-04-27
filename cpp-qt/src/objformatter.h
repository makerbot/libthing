#ifndef OBJFORMATTER_H
#define OBJFORMATTER_H

#include "formatter.h"

class ObjFormatter : public Formatter
{
public:
    Mesh* readMesh(QFile& inf);
    void writeMesh(QFile& outf, const Mesh& mesh);
    void writeMesh(QFile& outf, const SceneNode& node);
};

#endif // OBJFORMATTER_H

#ifndef OBJFORMATTER_H
#define OBJFORMATTER_H

#include "formatter.h"

class ObjFormatter : public Formatter
{
public:
    SmartMesh* readMesh(QFile& inf);
    void writeMesh(QFile& outf, const SmartMesh& mesh);
    void writeMesh(QFile& outf, const SceneNode& node);
};

#endif // OBJFORMATTER_H

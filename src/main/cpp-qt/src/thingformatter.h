#ifndef THINGFORMATTER_H
#define THINGFORMATTER_H

#include "formatter.h"
#include "json/json.h"

class ThingFormatter : public Formatter
{
public:
    Mesh* readMesh(QFile& inf);
    void writeMesh(QFile& outf, const SmartMesh& mesh);
    void writeMesh(QFile& outf, const SceneNode& node);
};

#endif // THINGFORMATTER_H

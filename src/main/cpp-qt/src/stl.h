#ifndef STL_H
#define STL_H

#include "formatter.h"
#include "objtree/scenenode.h"

class AsciiStlFormatter : public Formatter {
public:
  Mesh* readMesh(QFile& inf);
  void writeMesh(QFile& outf, const Mesh& mesh);
  void writeMesh(QFile& outf, const SceneNode& node);
};

class BinaryStlFormatter : public Formatter {
public:
  Mesh* readMesh(QFile& inf);
  void writeMesh(QFile& outf, const Mesh& mesh);
  void writeMesh(QFile& outf, const SceneNode& node);
};

#endif // STL_H

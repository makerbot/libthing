#ifndef PARTLIBRARY_H
#define PARTLIBRARY_H

///
/// The PartLibrary owns and maintains all the primitive
/// Parts (usually meshes) used in a model or plate.
/// Parts are currently considered to be immutable.
///

#include <QMap>
#include <QSharedPointer>
#include "mesh.h"

class PartLibrary {
private:
    static QMap<QString,QSharedPointer<SmartMesh> > m_parts;
public:
    static QSharedPointer<SmartMesh> getMesh(const QString filename);
    static void addMesh(const QString name, QSharedPointer<SmartMesh> mesh);
    static void addMesh(const QString name, SmartMesh* mesh);
    static const QString addFile(const QString path);
    static QString getOriginFile(const SmartMesh& mesh);
};


#endif // PARTLIBRARY_H

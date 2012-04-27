#include "partlibrary.h"
#include <QFileInfo>
#include "stl.h"
#include <fstream>
#include "formatter.h"
#include <QtDebug>

QMap<QString,QSharedPointer<Mesh> > PartLibrary::m_parts;

QSharedPointer<Mesh> PartLibrary::getMesh(const QString filename)
{
    if(!m_parts.contains(filename))
        addFile(filename);
    return m_parts[filename];
}

void PartLibrary::addMesh(const QString name, QSharedPointer<Mesh> mesh) {
    // Check for naming conflicts...
    m_parts[name] = mesh;
}

void PartLibrary::addMesh(const QString name, Mesh* mesh) {
    QSharedPointer<Mesh> p(mesh);
    addMesh(name,p);
}

const QString PartLibrary::addFile(const QString path) {
    QFileInfo fi(path);
    const QString absPath = fi.absoluteFilePath();
    QList<Formatter*> candidates = Formatter::getFactory().getFormatters(fi);
    QListIterator<Formatter*> i(candidates);
    Mesh* m = NULL;
    while (i.hasNext()) {
        try {
            QFile f(path);
            f.open(QFile::ReadOnly);
            m = i.next()->readMesh(f);
            f.close();
            m->setFilename(path);
            if(m->name().compare("") == 0);
                m->setName(QFileInfo(path).baseName());
            PartLibrary::addMesh(absPath,m);
            return absPath;
        } catch (ParseException& pe) {
            qDebug() << "Formatter X failed, trying next";
        }
    }
    qWarning() << fi.fileName() << "could not be read!";
    return NULL;
}

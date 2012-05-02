#include "partlibrary.h"
#include <QFileInfo>
#include "stl.h"
#include <fstream>
#include "formatter.h"
#include <QtDebug>

QMap<QString,QSharedPointer<SmartMesh> > PartLibrary::m_parts;

QSharedPointer<SmartMesh> PartLibrary::getMesh(const QString filename)
{
    if(!m_parts.contains(filename))
        addFile(filename);
    return m_parts[filename];
}

void PartLibrary::addMesh(const QString name, QSharedPointer<SmartMesh> mesh) {
    // Check for naming conflicts...
    m_parts[name] = mesh;
}

void PartLibrary::addMesh(const QString name, SmartMesh* mesh) {
    QSharedPointer<SmartMesh> p(mesh);
    addMesh(name,p);
}

const QString PartLibrary::addFile(const QString path) {
    QFileInfo fi(path);
    const QString absPath = fi.absoluteFilePath();
    QList<Formatter*> candidates = Formatter::getFactory().getFormatters(fi);
    QListIterator<Formatter*> i(candidates);
    SmartMesh* m = NULL;
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

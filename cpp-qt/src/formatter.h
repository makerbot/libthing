#ifndef FORMATTER_H
#define FORMATTER_H

#include "mesh.h"
#include "objtree/scenenode.h"
#include <QFile>
#include <QString>
#include <QMap>
#include <QList>
#include <QFileInfo>

class FormatterFactory;

class Formatter {
public:
    virtual Mesh* readMesh(QFile& inf) =0;
    virtual void writeMesh(QFile& outf, const Mesh& mesh) =0;
    virtual void writeMesh(QFile& outf, const SceneNode& node) =0;
    static FormatterFactory& getFactory();
};

class ParseException {
private:
    QString m_detail;
public:
    ParseException(QString detail) : m_detail(detail) {}
};

class FormatterFactory {
private:
    QMap<QString,QList<Formatter*> > m_formatterMap;
public:
    FormatterFactory();
    QList<Formatter*> getFormatters(QString format);
    QList<Formatter*> getFormatters(QFileInfo fi);

    // we use these to check supported formats instead of
    // getFormatters().count() > 0 because getFormatters
    // might insert an empty value into the map (hooray for side effects!)
    bool supports(QString format);
    bool supports(QFileInfo fi);
};

#endif // FORMATTER_H

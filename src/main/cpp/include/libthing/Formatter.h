#ifndef FORMATTER_H
#define FORMATTER_H

#include "libthing/Mesh.h"
//#include "objtree/scenenode.h"
//#include <QFile>
//#include <QString>
//#include <QMap>
//#include <QList>
//#include <QFileInfo>
#include <map>
#include <list>
#include <iostream>
namespace libthing {


class FormatterFactory;

class Formatter {
public:
	virtual Mesh* readMesh(std::istream& fh) = 0;
	virtual void writeMesh(std::ostream& outf, const Mesh& mesh) =0;
    static FormatterFactory& getFactory();
};

class ParseException {
//private:
//    QString m_detail;
//public:
//    ParseException(QString detail) : m_detail(detail) {}
};


class FormatterFactory {
private:
	std::map<std::string,std::list<Formatter*> > m_formatterMap;

public:
    FormatterFactory();

    std::list<Formatter*> getFormatters(std::string format);
    bool supports(std::string format);
};

};// end namespace libthing

#endif // FORMATTER_H

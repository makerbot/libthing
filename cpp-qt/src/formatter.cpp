#include "formatter.h"
#include "stl.h"
#include "objformatter.h"
#include "thingformatter.h"

FormatterFactory s_sharedFactory;

AsciiStlFormatter s_asciiStlFormatter;
BinaryStlFormatter s_binaryStlFormatter;
ObjFormatter s_objFormatter;
ThingFormatter s_thingFormatter;

FormatterFactory& Formatter::getFactory() {
    return s_sharedFactory;
}

FormatterFactory::FormatterFactory() {
    m_formatterMap["stl"].append(&s_asciiStlFormatter);
    m_formatterMap["ASCII STL (*.stl)"].append(&s_asciiStlFormatter);
    m_formatterMap["stl"].append(&s_binaryStlFormatter);
    m_formatterMap["Binary STL (*.stl)"].append(&s_binaryStlFormatter);
    m_formatterMap["obj"].append(&s_objFormatter);
    m_formatterMap["thing"].append(&s_thingFormatter);
}

QList<Formatter*> FormatterFactory::getFormatters(QString format) {
    return m_formatterMap[format];
}

QList<Formatter*> FormatterFactory::getFormatters(QFileInfo fi) {
    return m_formatterMap[fi.suffix().toLower()];
}

bool FormatterFactory::supports(QFileInfo fi)
{
    return supports(fi.suffix().toLower());
}

bool FormatterFactory::supports(QString format)
{
    return m_formatterMap.count(format) > 0;
}

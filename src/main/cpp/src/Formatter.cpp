#include "libthing/Formatter.h"
#include "libthing/StlFormatters.h"
//#include "objformatter.h"
//#include "thingformatter.h"

using namespace libthing;


FormatterFactory s_sharedFactory;

AsciiStlFormatter	s_asciiStlFormatter;
BinaryStlFormatter	s_binaryStlFormatter;
//ObjFormatter 		s_objFormatter;
//ThingFormatter 		s_thingFormatter;


FormatterFactory& Formatter::getFactory() {
    return s_sharedFactory;
}

FormatterFactory::FormatterFactory() {
    m_formatterMap["stl"].push_back(&s_asciiStlFormatter);
    m_formatterMap["ASCII STL (*.stl)"].push_back(&s_asciiStlFormatter);
    m_formatterMap["stl"].push_back(&s_binaryStlFormatter);
    m_formatterMap["Binary STL (*.stl)"].push_back(&s_binaryStlFormatter);
//    m_formatterMap["obj"].append(&s_objFormatter);
//    m_formatterMap["thing"].append(&s_thingFormatter);
}

std::list<Formatter*> FormatterFactory::getFormatters(std::string format)
{
	return m_formatterMap[format];
}

bool FormatterFactory::supports(std::string format)
{
	return m_formatterMap.count(format) > 0;
}


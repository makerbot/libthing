#-------------------------------------------------
#
# QtCreator project include file
#
# File is/was manually maintained for QT inclusion/use
#
#-------------------------------------------------

#JSON_CPP_SRC must be specified by parent calling
# .pro file for this to work properly. The default value
# is :
#LIBTHING_BASE = submodule/json-cpp

INCLUDEPATH += $$LIBTHING_BASE/qt-cpp/include
INCLUDEPATH += $$LIBTHING_BASE/cpp/include


SOURCES +=  $$LIBTHING_BASE/cpp/src/Vector3.cc\
	$$LIBTHING_BASE/cpp/src/Vector2.cc\
	$$LIBTHING_BASE/cpp/src/LineSegment2.cc\
	$$LIBTHING_BASE/cpp/src/Mesh.cc\
	$$LIBTHING_BASE/cpp/src/Scalar.cc\
	$$LIBTHING_BASE/cpp/src/StlReader.cc\
	$$LIBTHING_BASE/cpp/src/StlWriter.cc\
	$$LIBTHING_BASE/cpp/src/Triangle3.cc\
    $$LIBTHING_BASE/cpp/src/BoundingBox.cc

#HEADERS  +=$$JSON_CPP_SRC/include/json/assertions.h \


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


INCLUDEPATH += $$LIBTHING_BASE/qt-cpp/src
INCLUDEPATH += $$LIBTHING_BASE/../cpp/src


SOURCES +=  $$LIBTHING_BASE/../cpp/src/Vector3.cc\
    $$LIBTHING_BASE/../cpp/src/BoundingBox.cc\
    $$LIBTHING_BASE/../cpp/src/Scalar.cc

#HEADERS  +=$$JSON_CPP_SRC/include/json/assertions.h \


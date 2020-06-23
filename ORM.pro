#-------------------------------------------------
#
# Project created by QtCreator 2015-06-14T17:55:00
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = ORM
TEMPLATE = lib

DEFINES += ORM_LIBRARY
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG += c++14


COMPILER = ""
ARCH = ""

msvc {

 COMPILER = "MSVC2017"
 MSVC_VER = $$(VisualStudioVersion)
 equals(MSVC_VER, 16.0){
     COMPILER = "MSVC2019"
 }

    equals(QT_ARCH, "x86_64"){
        ARCH = "64bit"
    } else {
         ARCH = "32bit"
    }

}


SOURCES += \
    basebean.cpp \
    baserepository.cpp
#    ormforeign.cpp \
 #   ormtest.cpp \
   # model/beans/ormforeign.cpp \
   # model/beans/ormtest.cpp \
  #  model/beans/artist.cpp \
  #  model/beans/track.cpp \
  #  model/beans/gig.cpp
HEADERS +=\
    orm_global.h \
    basebean.h \
    baserepository.h
    #ormforeign.h \
   # ormtest.h \
   # model/beans/ormforeign.h \
   # model/beans/ormtest.h \
  #  model/beans/artist.h \
  #  model/beans/track.h \
  #  model/beans/gig.h

CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/SqlUtil2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_$${COMPILER}_$${ARCH}/release -lSqlUtil2
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/SqlUtil2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_$${COMPILER}_$${ARCH}/debug/ -lSqlUtil2

CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/QtCommon2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_$${COMPILER}_$${ARCH}/release/ -lQtCommon2
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/QtCommon2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_$${COMPILER}_$${ARCH}/debug/ -lQtCommon2

INCLUDEPATH += $$PWD/../SqlUtil2


INCLUDEPATH += $$PWD/../QtCommon2

unix {
    target.path = /usr/lib
    INSTALLS += target
}

gcc{
CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS_RELEASE -= -O
    QMAKE_CXXFLAGS_RELEASE -= -O1
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE *= -O3
}
}

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

CONFIG += c++14

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

win32 {
CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/SqlUtil2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MinGW_32bit/release/ -lSqlUtil2
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/SqlUtil2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MinGW_32bit/debug/ -lSqlUtil2

CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/QtCommon2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MinGW_32bit/release/ -lQtCommon2
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/QtCommon2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MinGW_32bit/debug/ -lQtCommon2
}

win32-msvc*  {
MSVC_VER = $$(VisualStudioVersion)
    equals(MSVC_VER, 15.0){
        CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/SqlUtil2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MSVC2017_64bit/release -lSqlUtil2
        else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/SqlUtil2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MSVC2017_64bit/debug/ -lSqlUtil2

        CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/QtCommon2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MSVC2017_64bit/release/ -lQtCommon2
        else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/QtCommon2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MSVC2017_64bit/debug/ -lQtCommon2
    }
}


INCLUDEPATH += $$PWD/../SqlUtil2


INCLUDEPATH += $$PWD/../QtCommon2

unix {
    target.path = /usr/lib
    INSTALLS += target
}


CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS_RELEASE -= -O
    QMAKE_CXXFLAGS_RELEASE -= -O1
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE *= -O3
}

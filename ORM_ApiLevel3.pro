#-------------------------------------------------
#
# Project created by QtCreator 2015-06-14T17:55:00
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = ORM_ApiLevel3
TEMPLATE = lib

DEFINES += ORM_LIBRARY
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG += c++1z

SOURCES += \
    baseentity.cpp \
    baserepository.cpp

HEADERS +=\
    baseentity.h \
    nullable.h \
    orm_global.h \
    baserepository.h
gcc {

contains(QT_ARCH, "x86_64"): {
CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/SqlUtil3-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MinGW_64_bit/release/ -lSqlUtil3
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/SqlUtil3-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MinGW_64_bit/debug/ -lSqlUtil3

CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/QtCommon2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MinGW_64_bit/release/ -lQtCommon2
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/QtCommon2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MinGW_64_bit/debug/ -lQtCommon2
} else {
CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/SqlUtil3-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MinGW_32bit/release/ -lSqlUtil3
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/SqlUtil3-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MinGW_32bit/debug/ -lSqlUtil3

CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/QtCommon2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MinGW_32bit/release/ -lQtCommon2
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/QtCommon2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MinGW_32bit/debug/ -lQtCommon2
}


}

win32-msvc*  {
MSVC_VER = $$(VisualStudioVersion)
    equals(MSVC_VER, 15.0){
        CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/SqlUtil3-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MSVC2017_64bit/release -lSqlUtil3
        else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/SqlUtil3-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MSVC2017_64bit/debug/ -lSqlUtil3

        CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/QtCommon2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MSVC2017_64bit/release/ -lQtCommon2
        else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/QtCommon2-Desktop_Qt_$${QT_MAJOR_VERSION}_$${QT_MINOR_VERSION}_$${QT_PATCH_VERSION}_MSVC2017_64bit/debug/ -lQtCommon2
    }
}


INCLUDEPATH += $$PWD/../SqlUtil3


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

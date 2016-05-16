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

CONFIG += c++11

SOURCES += \
    basebean.cpp
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
    beanquery.h
    #ormforeign.h \
   # ormtest.h \
   # model/beans/ormforeign.h \
   # model/beans/ormtest.h \
  #  model/beans/artist.h \
  #  model/beans/track.h \
  #  model/beans/gig.h

CONFIG(release, debug|release): LIBS += -L$$PWD/../build-SqlUtil2-Desktop_Qt_5_4_0_MinGW_32bit/release/ -lSqlUtil2
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-SqlUtil2-Desktop_Qt_5_4_0_MinGW_32bit/debug/ -lSqlUtil2


INCLUDEPATH += $$PWD/../SqlUtil2

CONFIG(release, debug|release): LIBS += -L$$PWD/../build-QtCommon-Desktop_Qt_5_4_0_MinGW_32bit/release/ -lQtCommon
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-QtCommon-Desktop_Qt_5_4_0_MinGW_32bit/debug/ -lQtCommon


INCLUDEPATH += $$PWD/../QtCommon

unix {
    target.path = /usr/lib
    INSTALLS += target
}

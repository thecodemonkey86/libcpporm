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

unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-SqlUtil2-Desktop_Qt_5_5_0_GCC_64bit/release/ -lSqlUtil2
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-SqlUtil2-Desktop_Qt_5_5_0_GCC_64bit/debug/ -lSqlUtil2


INCLUDEPATH += ../SqlUtil2

unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-QtCommon-Desktop_Qt_5_5_0_GCC_64bit/release/ -lQtCommon
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-QtCommon-Desktop_Qt_5_5_0_GCC_64bit/debug/ -lQtCommon


INCLUDEPATH += ../QtCommon

unix {
    target.path = /usr/lib
    INSTALLS += target
}

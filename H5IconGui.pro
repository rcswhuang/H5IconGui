#-------------------------------------------------
#
# Project created by QtCreator 2017-09-19T09:21:00
#
#-------------------------------------------------

QT += xml
QT += gui
QT += widgets
TARGET = H5IconGui
TEMPLATE = lib
win32{
    DESTDIR = $$(WF_ROOT)/lib
}

unix{
DESTDIR = /users/huangw/lib
}
DEFINES += H5ICONGUI_DLL

INCLUDEPATH += ../include
CONFIG += qt thread all
SOURCES += \
    hbaseobj.cpp \
    hiconarcitem.cpp \
    hiconcircleitem.cpp \
    hiconellipseitem.cpp \
    hicongraphicsitem.cpp \
    hiconlineitem.cpp \
    hiconobj.cpp \
    hiconpieitem.cpp \
    hiconpolygonitem.cpp \
    hiconpolylineitem.cpp \
    hiconrectitem.cpp \
    hiconselectionitem.cpp \
    hicontextitem.cpp \
    hiconsymbol.cpp \
    hicontemplate.cpp \
    hiconshowpattern.cpp \
    hdynamicobj.cpp \
    hiconcomplexitem.cpp \
    hgroupobj.cpp \
    hiconitemgroup.cpp \
    hiconrectobj.cpp \
    hiconcomplexobj.cpp

HEADERS +=\
    ../include/hiconapi.h \
    ../include/H5IconGui/hiconarcitem.h \
    ../include/H5IconGui/hiconcircleitem.h \
    ../include/H5IconGui/hiconellipseitem.h \
    ../include/H5IconGui/hicongraphicsitem.h \
    ../include/H5IconGui/hiconlineitem.h \
    ../include/H5IconGui/hiconobj.h \
    ../include/H5IconGui/hiconpieitem.h \
    ../include/H5IconGui/hiconpolygonitem.h \
    ../include/H5IconGui/hiconpolylineitem.h \
    ../include/H5IconGui/hiconrectitem.h \
    ../include/H5IconGui/hiconselectionitem.h \
    ../include/H5IconGui/hicontextitem.h \
    ../include/H5IconGui/h5iconguiexport.h \
    ../include/H5IconGui/hiconsymbol.h \
    ../include/H5IconGui/hicontemplate.h \
    ../include/H5IconGui/hiconshowpattern.h \
    ../include/H5IconGui/hdynamicobj.h \
    ../include/H5IconGui/hiconcomplexitem.h \
    ../include/H5IconGui/hbaseobj.h \
    ../include/H5IconGui/hgroupobj.h \
    ../include/H5IconGui/hiconitemgroup.h \
    ../include/H5IconGui/hiconrectobj.h \
    ../include/H5IconGui/hiconcomplexobj.h

win32{
    DEFINES += WIN32
}
unix {
    target.path = /users/huangw/lib
    INSTALLS += target
}

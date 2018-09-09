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

DESTDIR += ../lib

DEFINES += H5ICONGUI_DLL

INCLUDEPATH += \
               ../include \
               ../include/H5IconGui

CONFIG += qt warn_off thread exceptions
SOURCES += \
    hbaseobj.cpp \
    hiconarcitem.cpp \
    hiconcircleitem.cpp \
    hiconellipseitem.cpp \
    hicongraphicsitem.cpp \
    hiconlineitem.cpp \
    hiconpieitem.cpp \
    hiconpolygonitem.cpp \
    hiconpolylineitem.cpp \
    hiconselectionitem.cpp \
    hicontextitem.cpp \
    hiconsymbol.cpp \
    hicontemplate.cpp \
    hiconshowpattern.cpp \
    hdynamicobj.cpp \
    hiconcomplexitem.cpp \
    hgroupobj.cpp \
    hiconitemgroup.cpp \
    hrectangle.cpp \
    harc.cpp \
    hcircle.cpp \
    hellipse.cpp \
    hline.cpp \
    hpie.cpp \
    hpolygon.cpp \
    hpolyline.cpp \
    htext.cpp \
    hgraph.cpp \
    hiconobj.cpp \
    hiconrectangleitem.cpp

HEADERS +=\
    ../include/hiconapi.h \
    ../include/H5IconGui/hiconarcitem.h \
    ../include/H5IconGui/hiconcircleitem.h \
    ../include/H5IconGui/hiconellipseitem.h \
    ../include/H5IconGui/hicongraphicsitem.h \
    ../include/H5IconGui/hiconlineitem.h \
    ../include/H5IconGui/hiconpieitem.h \
    ../include/H5IconGui/hiconpolygonitem.h \
    ../include/H5IconGui/hiconpolylineitem.h \
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
    ../include/H5IconGui/hellipse.h \
    ../include/H5IconGui/hiconobj.h \
    ../include/H5IconGui/hrectangle.h \
    ../include/H5IconGui/hcircle.h \
    ../include/H5IconGui/harc.h \
    ../include/H5IconGui/hpie.h \
    ../include/H5IconGui/htext.h \
    ../include/H5IconGui/hline.h \
    ../include/H5IconGui/hpolygon.h \
    ../include/H5IconGui/hpolyline.h \
    ../include/H5IconGui/hgraph.h \
    ../include/H5IconGui/hiconrectangleitem.h

unix{
    UI_DIR = temp/ui
    MOC_DIR = temp/moc
    OBJECTS_DIR	= temp/obj
    LIBS += \

}

win32{
    UI_DIR = temp/ui
    MOC_DIR = temp/moc
    OBJECTS_DIR	= temp/obj
    LIBS += \
}

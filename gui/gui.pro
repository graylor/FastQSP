QT += core gui webkit webkitwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += multimedia
else: QT += phonon

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
CONFIG += c++11

TARGET = FastQSP
TEMPLATE = app

GIT_VERSION = $$system(git --git-dir $$PWD/../.git describe)
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
DEFINES += NOT_RUBY _UNICODE

win32: DEFINES+= _WIN

SOURCES += main.cpp\
    qsp_callback.cpp \
    fastqspwindow.cpp \
    qsp_htmlbuilder.cpp \
    local_requsts_proxy.cpp

HEADERS  += \
    qsp.h \
    qsp_default.h \
    qsp_callback.h \
    fastqspwindow.h \
    qsp_htmlbuilder.h \
    local_requsts_proxy.h

CONFIG(release, debug|release) {
    BUILDDIR = ../build/release
} 

CONFIG(debug, debug|release) {
    BUILDDIR = ../build/debug
}

DESTDIR = $$BUILDDIR/bin
OBJECTS_DIR = $$BUILDDIR/obj/gui
MOC_DIR = $$BUILDDIR/moc/gui

LIBS += -L$$DESTDIR -lqsp -loniguruma

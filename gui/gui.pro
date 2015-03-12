#-------------------------------------------------
#
# Project created by QtCreator 2014-02-18T21:18:59
#
#-------------------------------------------------

QT += core gui webkit webkitwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += multimedia
else: QT += phonon

TARGET = FastQSP
TEMPLATE = app

DEFINES += NOT_RUBY EXPORT _UNICODE

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

CONFIG(release, debug|release): DESTDIR = release
else: CONFIG(debug, debug|release): DESTDIR = debug

CONFIG(release, debug|release): OBJ_DIR = release
else: CONFIG(debug, debug|release): OBJ_DIR = debug


CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qsp/release/ -lqsp
else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qsp/debug/ -lqsp



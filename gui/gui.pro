#-------------------------------------------------
#
# Project created by QtCreator 2014-02-18T21:18:59
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets phonon

TARGET = FastQSP
TEMPLATE = app

DEFINES += NOT_RUBY EXPORT _WIN _UNICODE

SOURCES += main.cpp\
    qsp_callback.cpp \
    fastqspwindow.cpp

HEADERS  += \
    qsp.h \
    qsp_default.h \
    qsp_callback.h \
    fastqspwindow.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qsp/release/ -lqsp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qsp/debug/ -lqsp



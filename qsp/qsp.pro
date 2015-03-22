TEMPLATE = lib
TARGET = qsp
win32 {
    CONFIG += dll
    DEFINES+= _WIN
} 

win32-msvc* {
    QMAKE_LFLAGS_RELEASE += /MAP
    QMAKE_CFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /debug /opt:ref
    QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS,5.01
}
CONFIG += c++11

DEFINES += NOT_RUBY EXPORT _UNICODE _CRT_SECURE_NO_WARNINGS

CONFIG(release, debug|release) {
    BUILDDIR = ../build/release
} 

CONFIG(debug, debug|release) {
    BUILDDIR = ../build/debug    
}

DESTDIR = $$BUILDDIR/bin
OBJECTS_DIR = $$BUILDDIR/obj/qsp
MOC_DIR = $$BUILDDIR/moc/qsp

LIBS += -L$$DESTDIR -loniguruma

# Input
INCLUDEPATH += . ../oniguruma
HEADERS += actions.h \
           callbacks.h \
           codetools.h \
           coding.h \
           common.h \
           declarations.h \
           errors.h \
           game.h \
           locations.h \
           mathops.h \
           menu.h \
           objects.h \
           playlist.h \
           qsp.h \
           regexp.h \
           statements.h \
           text.h \
           variables.h \
           variant.h \
           bindings/bindings_config.h \
           bindings/android/android.h \
           bindings/default/qsp_default.h \
           bindings/flash/flash.h \
           bindings/java/java.h \
           time_qsp.h
SOURCES += \
    actions.cpp \
    callbacks.cpp \
    codetools.cpp \
    coding.cpp \
    common.cpp \
    errors.cpp \
    game.cpp \
    locations.cpp \
    mathops.cpp \
    menu.cpp \
    objects.cpp \
    playlist.cpp \
    regexp.cpp \
    statements.cpp \
    text.cpp \
    time_qsp.cpp \
    towlower.cpp \
    towupper.cpp \
    variables.cpp \
    variant.cpp \
    bindings/android/android_callbacks.cpp \
    bindings/android/android_coding.cpp \
    bindings/android/android_control.cpp \
    bindings/default/default_callbacks.cpp \
    bindings/default/default_coding.cpp \
    bindings/default/default_control.cpp \
    bindings/flash/flash_callbacks.cpp \
    bindings/flash/flash_coding.cpp \
    bindings/flash/flash_control.cpp \
    bindings/java/java_callbacks.cpp \
    bindings/java/java_coding.cpp \
    bindings/java/java_control.cpp

unix:QMAKE_CXXFLAGS+=-g

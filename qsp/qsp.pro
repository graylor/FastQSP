TEMPLATE = lib
TARGET = qsp
win32 {
    CONFIG += dll
    DEFINES+= _WIN
#    QMAKE_CFLAGS += /TP
} 

DEFINES += NOT_RUBY EXPORT _UNICODE

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
           memwatch.h \
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
SOURCES += actions.c \
           callbacks.c \
           codetools.c \
           coding.c \
           common.c \
           errors.c \
           game.c \
           locations.c \
           mathops.c \
           memwatch.c \
           menu.c \
           objects.c \
           playlist.c \
           regexp.c \
           statements.c \
           text.c \
           towlower.c \
           towupper.c \
           variables.c \
           variant.c \
           bindings/android/android_callbacks.c \
           bindings/android/android_coding.c \
           bindings/android/android_control.c \
           bindings/default/default_callbacks.c \
           bindings/default/default_coding.c \
           bindings/default/default_control.c \
           bindings/flash/flash_callbacks.c \
           bindings/flash/flash_coding.c \
           bindings/flash/flash_control.c \
           bindings/java/java_callbacks.c \
           bindings/java/java_coding.c \
           bindings/java/java_control.c \
           time_qsp.c

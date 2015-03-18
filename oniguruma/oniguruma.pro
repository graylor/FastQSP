TEMPLATE = lib
TARGET = oniguruma
win32 {
#    CONFIG += dll
    DEFINES+= _WIN
}

DEFINES += NOT_RUBY EXPORT _UNICODE

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

CONFIG(release, debug|release) {
    BUILDDIR = ../build/release
} 

CONFIG(debug, debug|release) {
    BUILDDIR = ../build/debug    
}

DESTDIR = $$BUILDDIR/bin
OBJECTS_DIR = $$BUILDDIR/obj/oniguruma
MOC_DIR = $$BUILDDIR/moc/oniguruma

# Input
INCLUDEPATH += .
HEADERS += config.h \
           oniguruma.h \
           regenc.h \
           regint.h \
           regparse.h \
           st.h

SOURCES += regcomp.c \
           regenc.c \
           regerror.c \
           regexec.c \
           regparse.c \
           regsyntax.c \
           regtrav.c \
           regversion.c \
           st.c \
           enc/ascii.c \
           enc/cp1251.c \
           enc/koi8_r.c \
           enc/unicode.c \
           enc/utf16_be.c \
           enc/utf16_le.c \
           enc/utf32_be.c \
           enc/utf32_le.c

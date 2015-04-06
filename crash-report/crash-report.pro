# ***************************************************************************
# Implemented using https://github.com/JPNaude/dev_notes/wiki/Using-Google-Breakpad-with-Qt
# as an example.
#
# Get Google Breakpad here: https://code.google.com/p/google-breakpad/
#
# The required breakpad sources have been copied into /google-breakpad-src in order to make
# integration with the application smooth and easy.
#
# To use source from Google Breakpad SVN checkout instead, change $$PWD/google-breakpad-src
# to path where it was checked out.
#
# ***************************************************************************

TEMPLATE = lib
TARGET = crash-report

win32 {
    CONFIG += dll
    DEFINES+= _WIN
}

win32-msvc* {
    QMAKE_LFLAGS_RELEASE += /MAP
    QMAKE_CFLAGS_RELEASE += /Zi /Zo
    QMAKE_LFLAGS_RELEASE += /debug /opt:ref
}

CONFIG += c++11
gcc:QMAKE_CXXFLAGS_CXX11 = -std=gnu++11

DEFINES += NOT_RUBY EXPORT _UNICODE _CRT_SECURE_NO_WARNINGS

CONFIG(release, debug|release) {
    BUILDDIR = ../build/release
}

CONFIG(debug, debug|release) {
    BUILDDIR = ../build/debug
}

DESTDIR = $$BUILDDIR/bin
OBJECTS_DIR = $$BUILDDIR/obj/crash-report
MOC_DIR = $$BUILDDIR/moc/crash-report

LIBS += -L$$DESTDIR

HEADERS +=  \
    crash-report.h
SOURCES += \
    crash-report.cpp

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/google-breakpad-src

# Common
HEADERS += $$PWD/google-breakpad-src/google_breakpad/common/minidump_format.h
HEADERS += $$PWD/google-breakpad-src/google_breakpad/common/minidump_size.h
HEADERS += $$PWD/google-breakpad-src/google_breakpad/common/breakpad_types.h
HEADERS += $$PWD/google-breakpad-src/common/basictypes.h
HEADERS += $$PWD/google-breakpad-src/common/byte_cursor.h
HEADERS += $$PWD/google-breakpad-src/common/convert_UTF.h
HEADERS += $$PWD/google-breakpad-src/common/language.h
HEADERS += $$PWD/google-breakpad-src/common/md5.h
HEADERS += $$PWD/google-breakpad-src/common/memory.h
HEADERS += $$PWD/google-breakpad-src/common/memory_range.h
HEADERS += $$PWD/google-breakpad-src/common/minidump_type_helper.h
HEADERS += $$PWD/google-breakpad-src/common/module.h
HEADERS += $$PWD/google-breakpad-src/common/scoped_ptr.h
HEADERS += $$PWD/google-breakpad-src/common/simple_string_dictionary.h
HEADERS += $$PWD/google-breakpad-src/common/string_conversion.h
HEADERS += $$PWD/google-breakpad-src/common/symbol_data.h
HEADERS += $$PWD/google-breakpad-src/common/unordered.h
HEADERS += $$PWD/google-breakpad-src/common/using_std_string.h

SOURCES += $$PWD/google-breakpad-src/common/convert_UTF.c
SOURCES += $$PWD/google-breakpad-src/common/language.cc
SOURCES += $$PWD/google-breakpad-src/common/md5.cc
SOURCES += $$PWD/google-breakpad-src/common/module.cc
SOURCES += $$PWD/google-breakpad-src/common/simple_string_dictionary.cc
SOURCES += $$PWD/google-breakpad-src/common/string_conversion.cc

# Windows
win32 {
    HEADERS += $$PWD/google-breakpad-src/common/windows/string_utils-inl.h
    HEADERS += $$PWD/google-breakpad-src/common/windows/guid_string.h
    HEADERS += $$PWD/google-breakpad-src/client/windows/handler/exception_handler.h
    HEADERS += $$PWD/google-breakpad-src/client/windows/common/ipc_protocol.h
    HEADERS += $$PWD/google-breakpad-src/client/windows/crash_generation/crash_generation_client.h

    SOURCES += $$PWD/google-breakpad-src/common/windows/string_utils.cc
    SOURCES += $$PWD/google-breakpad-src/common/windows/guid_string.cc
    SOURCES += $$PWD/google-breakpad-src/client/windows/handler/exception_handler.cc
    SOURCES += $$PWD/google-breakpad-src/client/windows/crash_generation/crash_generation_client.cc
}

# Linux
unix {
    HEADERS += $$PWD/google-breakpad-src/client/linux/dump_writer_common/ucontext_reader.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/dump_writer_common/thread_info.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/dump_writer_common/seccomp_unwinder.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/minidump_writer/cpu_set.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/minidump_writer/proc_cpuinfo_reader.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/handler/exception_handler.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/crash_generation/crash_generation_client.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/handler/minidump_descriptor.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/minidump_writer/minidump_writer.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/minidump_writer/line_reader.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/minidump_writer/linux_dumper.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/minidump_writer/linux_ptrace_dumper.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/minidump_writer/directory_reader.h
    HEADERS += $$PWD/google-breakpad-src/client/minidump_file_writer.h
    HEADERS += $$PWD/google-breakpad-src/client/minidump_file_writer-inl.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/microdump_writer/microdump_writer.h
    HEADERS += $$PWD/google-breakpad-src/client/linux/log/log.h
    HEADERS += $$PWD/google-breakpad-src/common/linux/linux_libc_support.h
    HEADERS += $$PWD/google-breakpad-src/common/linux/eintr_wrapper.h
    HEADERS += $$PWD/google-breakpad-src/common/linux/ignore_ret.h
    HEADERS += $$PWD/google-breakpad-src/common/linux/file_id.h
    HEADERS += $$PWD/google-breakpad-src/common/linux/memory_mapped_file.h
    HEADERS += $$PWD/google-breakpad-src/common/linux/safe_readlink.h
    HEADERS += $$PWD/google-breakpad-src/common/linux/guid_creator.h
    HEADERS += $$PWD/google-breakpad-src/common/linux/elfutils.h
    HEADERS += $$PWD/google-breakpad-src/common/linux/elfutils-inl.h
    HEADERS += $$PWD/google-breakpad-src/common/linux/elf_gnu_compat.h
    HEADERS += $$PWD/google-breakpad-src/third_party/lss/linux_syscall_support.h

    SOURCES += $$PWD/google-breakpad-src/client/linux/dump_writer_common/ucontext_reader.cc
    SOURCES += $$PWD/google-breakpad-src/client/linux/dump_writer_common/thread_info.cc
    SOURCES += $$PWD/google-breakpad-src/client/linux/dump_writer_common/seccomp_unwinder.cc
    SOURCES += $$PWD/google-breakpad-src/client/linux/crash_generation/crash_generation_client.cc
    SOURCES += $$PWD/google-breakpad-src/client/linux/handler/exception_handler.cc
    SOURCES += $$PWD/google-breakpad-src/client/linux/handler/minidump_descriptor.cc
    SOURCES += $$PWD/google-breakpad-src/client/linux/minidump_writer/minidump_writer.cc
    SOURCES += $$PWD/google-breakpad-src/client/linux/minidump_writer/linux_dumper.cc
    SOURCES += $$PWD/google-breakpad-src/client/linux/minidump_writer/linux_ptrace_dumper.cc
    SOURCES += $$PWD/google-breakpad-src/client/linux/microdump_writer/microdump_writer.cc
    SOURCES += $$PWD/google-breakpad-src/client/minidump_file_writer.cc
    SOURCES += $$PWD/google-breakpad-src/client/linux/log/log.cc
    SOURCES += $$PWD/google-breakpad-src/common/linux/linux_libc_support.cc
    SOURCES += $$PWD/google-breakpad-src/common/linux/file_id.cc
    SOURCES += $$PWD/google-breakpad-src/common/linux/memory_mapped_file.cc
    SOURCES += $$PWD/google-breakpad-src/common/linux/safe_readlink.cc
    SOURCES += $$PWD/google-breakpad-src/common/linux/guid_creator.cc
    SOURCES += $$PWD/google-breakpad-src/common/linux/elfutils.cc

    #breakpad app need debug info inside binaries
    QMAKE_CXXFLAGS+=-g
}

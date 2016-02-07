cmake_minimum_required(VERSION 3.0.2)

project(oniguruma C)
set(DEFINES NOT_RUBY EXPORT _UNICODE)
set(SOURCES
    regcomp.c
    regenc.c
    regerror.c
    regexec.c
    regparse.c
    regsyntax.c
    regtrav.c
    regversion.c
    st.c
    enc/ascii.c
    enc/cp1251.c
    enc/koi8_r.c
    enc/unicode.c
    enc/utf16_be.c
    enc/utf16_le.c
    enc/utf32_be.c
    enc/utf32_le.c
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} STATIC ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_DEFINITIONS "${DEFINES}"
    INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}"
)


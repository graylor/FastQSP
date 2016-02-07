cmake_minimum_required(VERSION 3.0.2)

project(qsp)

set(SOURCES
    actions.cpp
    callbacks.cpp
    codetools.cpp
    coding.cpp
    common.cpp
    errors.cpp
    game.cpp
    locations.cpp
    mathops.cpp
    menu.cpp
    objects.cpp
    playlist.cpp
    regexp.cpp
    statements.cpp
    text.cpp
    time_qsp.cpp
    towlower.cpp
    towupper.cpp
    variables.cpp
    variant.cpp
    bindings/android/android_callbacks.cpp
    bindings/android/android_coding.cpp
    bindings/android/android_control.cpp
    bindings/default/default_callbacks.cpp
    bindings/default/default_coding.cpp
    bindings/default/default_control.cpp
    bindings/flash/flash_callbacks.cpp
    bindings/flash/flash_coding.cpp
    bindings/flash/flash_control.cpp
    bindings/java/java_callbacks.cpp
    bindings/java/java_coding.cpp
    bindings/java/java_control.cpp
)
set(DEFINES NOT_RUBY EXPORT _UNICODE _CRT_SECURE_NO_WARNINGS)
set(INCLUDES ${PROJECT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/oniguruma)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} oniguruma)
set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_DEFINITIONS "${DEFINES}"
    INCLUDE_DIRECTORIES "${INCLUDES}"
)


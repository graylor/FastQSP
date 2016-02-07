cmake_minimum_required(VERSION 3.0.2)

project(FastQsp CXX)
set(SOURCES
    main.cpp
    qsp_callback.cpp
    fastqspwindow.cpp
    qsp_htmlbuilder.cpp
    local_requsts_proxy.cpp
)
set(DEFINES NOT_RUBY _UNICODE GIT_VERSION=${GIT_VERSION})
if (WIN32)
    list(APPEND DEFINES _WIN)
endif(WIN32)
set(INCLUDES
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/oniguruma
    ${CMAKE_SOURCE_DIR}/qsp
)
set(LIBRARIES qsp oniguruma)

# Qt stuff
set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Gui Network Multimedia WebKit WebKitWidgets)
if (Qt5_FOUND)
    message(STATUS "Using Qt5")
    list(APPEND INCLUDES
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS}
        ${Qt5Multimedia_INCLUDE_DIRS}
        ${Qt5WebKit_INCLUDE_DIRS}
        ${Qt5WebKitWidgets_INCLUDE_DIRS}
    )
    list(APPEND LIBRARIES
        ${Qt5Core_LIBRARIES}
        ${Qt5Gui_LIBRARIES}
        ${Qt5Network_LIBRARIES}
        ${Qt5Multimedia_LIBRARIES}
        ${Qt5WebKit_LIBRARIES}
        ${Qt5WebKitWidgets_LIBRARIES}
    )
    list(REMOVE_DUPLICATES INCLUDES)
    list(REMOVE_DUPLICATES LIBRARIES)
else()
    message(STATUS "Qt5 is not found fallback to Qt4")
    find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork QtWebKit REQUIRED)
    find_package(Phonon REQUIRED)
    list(APPEND INCLUDES
        ${QT_INCLUDES}
        ${PHONON_INCLUDES}
    )
    list(APPEND LIBRARIES
        ${QT_QTCORE_LIBRARY}
        ${QT_QTGUI_LIBRARY}
        ${QT_QTNETWORK_LIBRARY}
        ${QT_QTWEBKIT_LIBRARY}
        ${PHONON_LIBRARIES}
    )
    list(REMOVE_DUPLICATES INCLUDES)
    list(REMOVE_DUPLICATES LIBRARIES)
endif(Qt5_FOUND)

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_DEFINITIONS "${DEFINES}"
    INCLUDE_DIRECTORIES "${INCLUDES}"
)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})


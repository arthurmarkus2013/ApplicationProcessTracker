cmake_minimum_required(VERSION 3.5)

project(ApplicationProcessTracker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Sql)

set(TS_FILES ApplicationProcessTracker_en_US.ts)

set(PROJECT_SOURCES
        Entry.h
        businesslogic.h
        businesslogic.cpp
        addentrydialog.h
        addentrydialog.cpp
        addentrydialog.ui
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ApplicationProcessTracker
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
endif()

qt_add_translations(ApplicationProcessTracker TS_FILES ${TS_FILES} RESOURCE_PREFIX "/translations")

target_link_libraries(ApplicationProcessTracker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql ${CONAN_LIBS})

set_target_properties(ApplicationProcessTracker PROPERTIES
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ApplicationProcessTracker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ApplicationProcessTracker)
endif()

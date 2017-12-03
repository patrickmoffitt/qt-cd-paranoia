cmake_minimum_required(VERSION 2.8.12)
# CMAKE_OSX_DEPLOYMENT_TARGET Must be set before project() is called.
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11")
project("qt-cd-paranoia")
set(PROJECT_NAME "qt-cd-paranoia")

set(RELEASE ON)
set(DEVELOPMENT OFF)
set(APPLE ON)
set(CYGWIN OFF)

if(DEVELOPMENT)
    set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-debug/")
    add_definitions(-DSTD_ERROR_LOG)
endif()

# Release configuration builds an application.app bundle inside a DMG.
if(RELEASE)
    set(MAJOR "0")
    set(MINOR "1")
    set(PATCH "1")
    set(AUTHOR "Patrick Moffitt")
    set(VERSION "${MAJOR}.${MINOR}.${PATCH}")
    set(APP_NAME "Qt CD Paranoia")
    set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-release/")

    set(CPACK_MONOLITHIC_INSTALL "1")
    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-release")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${APP_NAME}")
    set(CPACK_PACKAGE_VENDOR "${AUTHOR}")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
    set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PATCH}")
    set(CPACK_PACKAGE_FILE_NAME "qt-cd-paranoia-${VERSION}")
    if(APPLE)
        set(CMAKE_PREFIX_PATH /usr/local/Qt/5.9.2/clang_64//lib/cmake/Qt5/)
        set(ICNS_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/images/if_cdaudio_unmount_18274.icns")
        set(MACOSX_BUNDLE_BUNDLE_NAME "${APP_NAME}")
        set(MACOSX_BUNDLE_EXECUTABLE_NAME "${MACOSX_BUNDLE_BUNDLE_NAME}")
        set(MACOSX_BUNDLE_INFO_STRING "Copyright (C) 2017 ${AUTHOR}")
        set(MACOSX_BUNDLE_COPYRIGHT "${MACOSX_BUNDLE_INFO_STRING}")
        set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.moffitt.qtcdparanoia")
        set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}")
        set(MACOSX_BUNDLE_LONG_VERSION_STRING "${VERSION}")
        set(MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION}")
        set(MACOSX_BUNDLE_ICON_FILE "if_cdaudio_unmount_18274.icns")

        set(CPACK_BUNDLE_STARTUP_COMMAND
                "${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-release/qt-cd-paranoia")
        set(CPACK_DMG_FORMAT "UDRO")
        set(CPACK_PACKAGE_ICON "${ICNS_FILE_PATH}")
        set_source_files_properties(${CPACK_PACKAGE_ICON} PROPERTIES
                MACOSX_PACKAGE_LOCATION "Resources")
        set(CPACK_DMG_BACKGROUND_IMAGE
                "${CMAKE_CURRENT_SOURCE_DIR}/images/if_cdaudio_unmount_18274_512.png")
        set(CPACK_DMG_DS_STORE "${CMAKE_CURRENT_SOURCE_DIR}/release/osx/DS_Store.in")
        set(CPACK_BUNDLE_NAME "${APP_NAME}")
        set(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/release/osx/Info.plist.in")
        set(CPACK_BUNDLE_ICON "${ICNS_FILE_PATH}")
        set(CPACK_GENERATOR "Bundle")

        set(CMAKE_LIBRRY_PATH "/usr/local/lib/")
        include_directories(BEFORE "/usr/include" "/usr/local/include")
        find_package(Boost 1.65.1 COMPONENTS filesystem REQUIRED)

    endif()
    if(CYGWIN)
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll.a")
        set(BUILD_SHARED_LIBS "0")
        set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/images/if_cdaudio_unmount_18274.ico")
        set(CPACK_PACKAGE_EXECUTABLES "${CPACK_PACKAGE_FILE_NAME}" "${CPACK_PACKAGE_FILE_NAME}")
        set(CPACK_GENERATOR "TGZ")
        set(CMAKE_LIBRRY_PATH "c:/cygwin64/lib" "C:/cygwin64/usr/local/lib")
        include_directories(BEFORE "c:/cygwin64/usr/include")
        set(BOOST_INCLUDE_DIR "c:/cygwin64/usr/include/boost")
        find_package(Boost 1.63.0 COMPONENTS filesystem REQUIRED)
    endif()
    include(CPack)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_FIND_LIBRARY_PREFIXES "lib")

project(qt-cd-paranoia LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick Concurrent Widgets REQUIRED)

file (GLOB Sources *.cpp)
file (GLOB Headers *.hpp)
file (GLOB Resources *.qrc)

QT5_ADD_RESOURCES (ResourceSources ${Resources})

set (CMAKE_CXX_FLAGS " -v")

set(CMAKE_STATIC_LINKER_FLAGS " -v -search_paths_first")

find_library(CDIO name cdio PATHS ${CMAKE_LIBRRY_PATH})
find_library(CDDB name cddb PATHS ${CMAKE_LIBRRY_PATH})
find_library(CDIO_PARANOIA name cdio_paranoia PATHS ${CMAKE_LIBRRY_PATH})
find_library(CDIO_CDDA name cdio_cdda PATHS ${CMAKE_LIBRRY_PATH})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()


message(STATUS "CMake found: ${CDIO} ${CDDB} ${CDIO_PARANOIA} ${CDIO_CDDA} ${Boost_INCLUDE_DIRS}")

add_executable(${PROJECT_NAME} ${CPACK_PACKAGE_ICON} ${Sources} ${Headers} ${Resources})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${CDIO} ${CDDB} ${CDIO_PARANOIA}
        ${CDIO_CDDA} Qt5::Core Qt5::Quick Qt5::Concurrent Qt5::Widgets)

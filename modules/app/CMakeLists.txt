project(KnotEditor)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets OpenGLWidgets Network REQUIRED)

file(STRINGS "../VERSION" PROJECT_VERSION)
add_definitions(-DVERSION="${PROJECT_VERSION}")
include(version)
setup_package_version_variables(PROJECT)

include_directories(../core/src)
include_directories(../math/src)

file(GLOB CXX_SOURCES src/*.cpp)
file(GLOB QRC_SOURCES res/*.qrc)
set(SOURCES ${CXX_SOURCES} ${QRC_SOURCES})

# Project meta info
set(COPYRIGHT "Copyright (c) 1995-2021, Mathesis Software")
set(VENDOR "Mathesis Software")
set(EMAIL "mad@mathesis.fun")
set(PROJECT_DESCRIPTION "A tool for drawing 2d diagrams, 3d knots, Seifert surfaces. Computes some knot properties and invariants. Provides access to a network database of knots with <= 16 crossings.")


if (APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    file(GLOB RES_ICONS "${CMAKE_CURRENT_SOURCE_DIR}/res/images/*.icns")
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} ${RES_ICONS})
    set_source_files_properties(${RES_ICONS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION . RUNTIME DESTINATION .)

    set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE ON
            MACOSX_BUNDLE_GUI_IDENTIFIER com.fbreader.${PROJECT_NAME}
            MACOSX_BUNDLE_BUNDLE_IDENTIFIER com.fbreader.${PROJECT_NAME}
            MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} ${COPYRIGHT}"
            MACOSX_BUNDLE_ICON_FILE trefoil.icns
            MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
            MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
            MACOSX_BUNDLE_COPYRIGHT "${COPYRIGHT}"
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
            )
    include(Packaging)
else ()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif ()

target_link_libraries(${PROJECT_NAME} core math Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGLWidgets Qt6::Network)

# If you don't like default install location, uncomment next line and set it to desired value
#set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "..." FORCE)

if (UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
elseif (WIN32)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
endif ()

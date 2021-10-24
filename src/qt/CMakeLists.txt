project(KnotEditor)
include_directories(../ke ../math)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets OpenGLWidgets Network REQUIRED)

file(STRINGS "../VERSION" PROJECT_VERSION)
add_definitions(-DVERSION="${PROJECT_VERSION}")
include(version)
setup_package_version_variables(PROJECT)

set(SOURCES AboutDialog.cpp
        Application.cpp
        BaseWindow.cpp
        DiagramPropertiesDialog.cpp
        DiagramWidget.cpp
        DiagramWindow.cpp
        FileIconProvider.cpp
        GLWidget.cpp
        IPCController.cpp
        KnotMathDialog.cpp
        KnotOptionsDialog.cpp
        KnotWidget.cpp
        KnotWidget_smooth.cpp
        KnotWindow.cpp
        LibraryModel.cpp
        LibraryWindow.cpp
        main.cpp
        NetworkManager.cpp
        Preview.cpp
        StartWindow.cpp
        Window.cpp
        data.qrc
        images.qrc
        )

# If you don't like default install location, uncomment next line and set it to desired value
#set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "..." FORCE)

# Project meta info
set(COPYRIGHT "Copyright (c) 2021 geometer@fbreader.org")
set(VENDOR "Fbreader.org")
set(EMAIL "<geometer@fbreader.org>")
set(PROJECT_DESCRIPTION "An app for drawing knot diagrams, converting them to 3d knots, smoothing the knots etc.")

if (UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
elseif (WIN32)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
elseif (APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION . RUNTIME DESTINATION .)
    set(APP_ICON_MACOSX ${CMAKE_SOURCE_DIR}/images/trefoil.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

#    set_source_files_properties(${CMAKE_SOURCE_DIR}/images/trefoil.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
#    set_source_files_properties(${CMAKE_SOURCE_DIR}/images/file_dgr.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
#    set_source_files_properties(${CMAKE_SOURCE_DIR}/images/file_knt.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

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
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} $APP_ICON_MACOSX)
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif ()

add_dependencies(${PROJECT_NAME} rapidjson)
target_link_libraries(${PROJECT_NAME} ke math Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGLWidgets Qt6::Network)
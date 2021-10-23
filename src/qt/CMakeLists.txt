project (KnotEditor)
include_directories( ../ke ../math)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets OpenGLWidgets Network REQUIRED)

file(STRINGS "../VERSION" ver)
set(PROJECT_VERSION "\"${ver}\"")

include (version)
setup_package_version_variables(PROJECT)

add_definitions(-DVERSION=${PROJECT_VERSION})

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

if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
elseif(APPLE)
    set(CMAKE_MACOSX_BUNDLE ON)
endif()

add_executable( ${PROJECT_NAME} ${SOURCES})
add_dependencies( ${PROJECT_NAME} rapidjson)
target_link_libraries(${PROJECT_NAME} ke math Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGLWidgets Qt6::Network)

set(target ${PROJECT_NAME})

if(UNIX AND NOT APPLE)
    install(TARGETS ${target} RUNTIME DESTINATION bin)
elseif(WIN32)
    install(TARGETS ${target} RUNTIME DESTINATION bin)
elseif(APPLE)
    set(CMAKE_MACOSX_BUNDLE ON)
    set(CMAKE_MACOSX_RPATH ON)
    install(TARGETS ${target} BUNDLE DESTINATION .
            RUNTIME DESTINATION .)
    set_source_files_properties(${CMAKE_SOURCE_DIR}/images/trefoil.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_target_properties(${target}
            PROPERTIES
            MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
            MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} Copyright (c) 2021 geometer@fbreader.org"
            MACOSX_BUNDLE_ICON_FILE trefoil.icns
            MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_NAME}"
            MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
            MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
            MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2021 geometer@fbreader.org"
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
            )
endif()

# Components:
if(CMAKE_BUILD_TYPE_UPPER MATCHES "^(DEBUG|RELWITHDEBINFO)$")
    set(CPACK_STRIP_FILES FALSE)
else()
    set(CPACK_STRIP_FILES TRUE)
endif()

include(Packaging)

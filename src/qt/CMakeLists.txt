project (KnotEditor)
include_directories( ../ke ../math)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets OpenGLWidgets Network REQUIRED)

file(STRINGS "../VERSION" ver)
set(ver "\"${ver}\"")
add_definitions(-DVERSION=${ver})

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

if (APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES})
    set_target_properties(${PROJECT_NAME} PROPERTIES
            BUNDLE True
            MACOSX_BUNDLE_GUI_IDENTIFIER com.fbreader.${PROJECT_NAME}
            MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
            MACOSX_BUNDLE_BUNDLE_VERSION ${ver}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${ver}
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
            )

    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
    install(CODE  "execute_process(COMMAND ${MACDEPLOYQT_EXECUTABLE} ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app -dmg WORKING_DIRECTORY ${CPACK_PACKAGE_DIRECTORY} -always-overwrite)")
else()
    add_executable( ${PROJECT_NAME} ${SOURCES})
endif()

add_dependencies( ${PROJECT_NAME} rapidjson)

target_link_libraries(${PROJECT_NAME} ke math Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGLWidgets Qt6::Network)

# If you don't like default install location, uncomment next line and set it to desired value
#set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "..." FORCE)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        BUNDLE	DESTINATION bin )

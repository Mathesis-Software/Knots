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

add_executable( ${PROJECT_NAME} ${SOURCES})
add_dependencies( ${PROJECT_NAME} rapidjson)

target_link_libraries(${PROJECT_NAME} ke math Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGLWidgets Qt6::Network)

# If you don't like default install location, uncomment next line and set it to desired value
#set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "..." FORCE)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

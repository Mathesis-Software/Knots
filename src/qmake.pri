QMAKE_CXX = ccache $$QMAKE_CXX
QMAKE_CXXFLAGS += -Wno-unused-command-line-argument

CONFIG += c++17

INCLUDEPATH += /usr/local/Cellar/rapidjson/1.1.0/include

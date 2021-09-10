# KnotEditor

## Description

Originally written in 199x. One of my first programming projects.

Refurbished in July-September 2021. Special thanks to [@temper8]( https://github.com/temper8 ) for OpenGL smoothing solution.

Written in C++/Qt6, tested on Linux (Ubuntu 20.04) + Qt 6.1.3, and MacOSX (Monterey 12.0 beta, Intel) + Qt 6.1.3.

An app for

* drawing knot diagrams
* converting them to 3d knots
* smoothing the knots
* computing and showing Seifert surfaces
* computing some knot invariants
* etc.

For possible improvements list please refer [the TODO](https://github.com/geometer/KnotEditor/blob/master/doc/TODO.md). Please, note: this is not a roadmap. Just a collection of thoughts. Maybe I implement something from the list in the future, maybe not.

## Build & installation instructions

### Linux

#### Build dependencies

C++ compiler (the code uses some C++17 features), Qt6, OpenGL, rapidjson.

To install build dependencies on Ubuntu 20.04:
```
sudo apt install g++ make ccache libgl-dev rapidjson-dev
```

Qt6 is available from [the Qt site](https://www.qt.io/download). KnotEditor is truly open-source, so feel free to use the open-source Qt edition for this app.

#### To build
```
cd KnotEditor
/<your-path-to-qt6>/<version>/gcc_64/bin/qmake
```
(e.g., `~/Qt6/6.1.3/gcc_64/bin/qmake`)
```
make
```

#### To install
By default, the app is installed to `/usr/local/bin/KnotEditor`. Single binary, no additional files. Alternatively, you can change the `INSTALL_PREFIX` var in `src/qmake.pri`.
```
sudo make install
```

#### To run
```
KnotEditor
```

### MacOS X

You can build it from the sources, as described below. Alternatively, you can

#### Download disk image with precompiled app

[DMG Image: KnotEditor for MacOS Monterey (Intel)](https://github.com/geometer/KnotEditor/blob/master/pre-built/KnotEditor-1.0.20210910.dmg?raw=true)

#### Build dependencies

Clang C++ compiler, ccache, Qt6, rapidjson.

To install rapidjson use:
```
brew install rapidjson
```

Qt6 is available from [the Qt site](https://www.qt.io/download). KnotEditor is truly open-source, so feel free to use the open-source Qt edition for this app.

#### To build
```
cd KnotEditor
/<your-path-to-qt6>/<version>/macos/bin/qmake
```
(e.g., `~/Qt6/6.1.3/macos/bin/qmake`)
```
make
```

#### To install

After a successful build, the app is in ```src/qt``` folder. Open the folder in Finder and drag-n-drop the app to the Applications folder. Please note that Qt frameworks are not included in the app. If you plan to delete Qt installation from your Mac, run
```
/<your-path-to-qt6>/<version>/macos/bin/macdeployqt src/qt/KnotEditor.app
```
before installation.

## Screenshots

![Torus 5x2 knot, from a diagram to a smooth image](https://github.com/geometer/KnotEditor/blob/master/screenshots/2021-09-10.png?raw=true)
![Knot library](https://github.com/geometer/KnotEditor/blob/master/screenshots/2021-09-10-library.png?raw=true)

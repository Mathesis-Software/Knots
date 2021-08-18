# KnotEditor

### Description

Originally written in 199x. One of my first programming projects.

Partially refurbished in July-August 2021. Special thanks to [@temper8]( https://github.com/temper8 ) for OpenGL smoothing solution.

An app for

* drawing knot diagrams
* converting them to 3d knots
* smoothing the knots
* computing and showing Seifert surfaces
* computing some knot invariants
* etc.

For possible improvements list please refer [the TODO](https://github.com/geometer/KnotEditor/blob/master/doc/TODO.md). Please, note: this is not a roadmap. Just a collection of thoughts. Maybe I implement something from the list in the future, maybe not.

### Build & installation instruction

Build dependencies: C++ compiler (the code uses some C++17 features), Qt5, OpenGL, rapidjson.

To install build dependencies on Ubuntu 20.04:
```
sudo apt install g++ make ccache qtbase5-dev qtbase5-dev-tools libqt5opengl5-dev libgl-dev rapidjson-dev
```

To build:
```
$ make
```
On a non-Ubuntu system, you might need to edit `src/config.mk` to configure the build.

By default, the app is installed to `/usr/local/KnotEditor`, with script `knoted` in `/usr/local/bin`. Alternatively, you can change the `INSTPREFIX` var in `src/config.mk`.
```
$ sudo make install
```

To run:
```
$ knoted
```

![A screenshot](https://github.com/geometer/KnotEditor/blob/master/screenshots/2021-08-17.png?raw=true)

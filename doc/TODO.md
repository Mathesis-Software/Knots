### Mac specific

* Application icon
* restore other subdir compilation

### Bug fixing

* GL: widget resizing blinks in black; probably, setBackgroundColor for something helps
* Multiple scratches in Seifert surfaces
* Library window: artifacts on top of tab widget
* Rare crash in 3d mode

### Code structure

* consistent source file names
* replace linked list in seifert structures with std::list
* drop own Polynom & SquareMatrix classes; use something standard

### User interface

* new diagram view (undoable) actions: center, scale
* knot library: implement search
* knot and diagram: store extra info, like description or keywords; use it for search in the library
* recent documents list in the menu
* open windows list
* knot window: move length & number of points into "Visual options" dialogue
* create default settings storage and settings dialogue (all the same parameters as in knot visual options dialogue + maybe something els)
* "save file dialogue": do not use read-only directory by default
* save latest file directory between runs
* save window sizes between runs
* re-think "+" and "-" buttons for Seifert surface movement: it is currently absolutely unclear for a user
* disclose "Shift" and "Alt" magic for knot (mouse) movements to a user; maybe, there is a "standard" solution?
* undo/redo in knot window
* export image: "draw background" option

### Technical feature improvements

* move invariants (and other params) computations to a separate thread, with progress indication and a way to interrupt
* replace "msleep(20)" in the smoothing thread with something more clear
* migrate from OpenGL to Vulkan/Metal/DirectX
* do not save the app data in QtProject.conf, use own name instead

### Math feature improvements

* Vassiliev invariants: replace "gauss products" with numerically calculated integrals
* Use other form of energy for smoothing (?) Moebius energy has a subspace of extremum points, not a single extremum
* Seifert surfaces: understand the logic and probably improve

### Possible future development

* DT code => diagram (orthgonal? beautified?)
* multi-component links
* combinatorial invariants/params for diagrams
* saving 3D scene in VRML (or what is the standard de facto at the moment?)
* diagram editor: disallow line intersections during vertex moving/deleting

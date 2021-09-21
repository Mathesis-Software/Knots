### Mac specific

* file type association: check json structure?
* system file previews
* Tab and hotkeys do not work in QMessageDialog (e.g., saving file on exit)
* automatically build an installation package including Qt frameworks

### Bug fixing

* Parameters window for T(3,16) hangs the app
* GL: widget resizing blinks in black; probably, setBackgroundColor for something helps
* Multiple scratches in Seifert surfaces
* Rare crash in 3d mode
* Build config does not rebuild the main app automatically if a library file has been changed

### Code structure

* consistent source file names
* replace linked list in seifert structures with std::list
* drop own Polynom & SquareMatrix classes; use something standard

### User interface

#### High priority
* update the site: main page, dowloads section, acknowledgements
* "recent documents" tab in the library
* new diagram view (undoable) actions: center, scale
* re-think "+" and "-" buttons for Seifert surface movement: it is currently absolutely unclear for a user
* network search: show total number of matches
* network search: better way to display network errors

#### Medium priority
* knot view: auto-scale to fit window size
* knot and diagram: store extra info, like description or keywords; use it for search in the library
* library view: show some description/comments in tooltip over the item
* list of open windows (in menu?)
* disclose "Shift" and "Alt" magic for knot (mouse) movements to a user; maybe, there is a "standard" solution?
* undo/redo in knot window
* export image: "draw background" option
* save & restore windows z-order

#### Low priority
* search help action: hilite icon if under mouse
* knot library: save positions in lists between runs
* knot window: move length & number of points into "Visual options" dialogue
* create settings dialogue (all the same parameters as in knot visual options dialogue + maybe something els)

### Technical feature improvements

* move invariants (and other params) computations to a separate thread, with progress indication and a way to interrupt
* replace "msleep(20)" in the smoothing thread with something more clear
* migrate from OpenGL to Vulkan/Metal/DirectX

### Math feature improvements

* Vassiliev invariants: replace "gauss products" with numerically calculated integrals
* Use other form of energy for smoothing (?) Moebius energy has a subspace of extremum points, not a single extremum
* Seifert surfaces: understand the logic and probably improve

### Possible future development

* beautiful diagrams rendering like Knotscape's one
* Knot detection like https://knotinfo.math.indiana.edu/homelinks/knotfinder.php does
* multi-component links
* more combinatorial invariants/params for diagrams
* saving 3D scene in VRML (or what is the standard de facto at the moment?)
* diagram editor: disallow line intersections during vertex moving/deleting

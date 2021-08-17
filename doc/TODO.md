### Bug fixing

* GL: widget resizing blinks in black; probably, setBackgroundColor for something helps

### Code structure

* move everything into namespaces
* consistent source file names
* drop "qt/setValue" code
* (?) merge ManagerWindow, knotWindow, and diagramWindow into single class with replaceable content + menu + toolbars; this makes first opening smooth
* (?) save rotation matrix (currently knot is saved "rotated", with new points coordinates)
* move all data and as much as possible code from knotWindow to KnotWidget
* replace linked list in seifert structures with std::list

### User interface

* save list of opened windows between runs; reopen them
* recent documents list in the menu
* open windows list
* knot window: combine tunable parameters (colours, thickness, length, number of points) into a single dialogue
* create default settings storage and settings dialogue (all the same parameters as in previous item + maybe something els)
* "save file dialogue": do not use read-only directory by default
* save latest file directory between runs
* save window sizes between runs
* printing: drop or re-implement (currently it is disabled)
* add exporting images to PNG
* re-think "+" and "-" buttons for Seifert surface movement: it is currently absolutely unclear for a user
* disclose "Shift" and "Alt" magic for knot (mouse) movements to a user; maybe, there is a "standard" solution?
* pass diagram/knot files to open as parameters in command line
* undo/redo in knot window

### Technical feature improvements

* move invariants (and other params) computations to a separate thread, with progress indication and a way to interrupt
* replace "msleep(20)" in the smoothing thread with something more clear

### Math feature improvements

* Vassiliev invariants: replace "gauss products" with numerically calculated integrals
* Use other form of energy for smoothing (?) Moebius energy has a subspace of extremum points, not a single extremum
* Seifert surfaces: understand the logic and probably improve

### Data

* drop the zoo (e.g., the folder "300"), provide good single version for each knot
* (?) put data inside the executable, to be sure we need no special installation

### Possible future development

* multi-component links 
* combinatorial invariants/params for diagrams
* saving 3D scene in VRML (or what is the standard de facto at the moment?)

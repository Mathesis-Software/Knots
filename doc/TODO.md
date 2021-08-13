### Bug fixing

* GL: smoothing with number of steps updates status line from wrong thread
* isSaved() for knot does not respect the rotation matrix
* GL: widget resizing blinks in black; probably, setBackgroundColor for something helps

### Code structure

* move everything into namespaces
* consistent file names
* drop "qt/setValue" code

### User interface

* save list of opened windows between runs; reopen them
* replace current "manager" with a normal window with two BIG buttons: "create new diagram" and "open existing diagram/knot file"; only show it if there are no other windows
* implement few useful hotkeys (e.g., Ctrl/Meta-Q for exiting the app), drop most of existing shortcuts
* knot window: combine tunable parameters (colours, thickness, length, number of points) into a single dialogue
* create default settings storage and settings dialogue (all the same parameters as in previous item + maybe something els)
* "save file dialogue": do not use non-writeable directory by default
* save "visual" features like colours, knot thickness, window size, etc.
* printing: drop or re-implement (currently it is disabled)

### Technical feature improvements

* move invariants (and other params) computations to a separate thread, with progress indication and a way to interrupt
* replace "msleep(20)" in the smoothing thread with something more clear

### Math feature improvements

* Vassiliev invariants: replace "gauss products" with numerically calculated integrals
* Use other form of energy for smoothing (?) Moebius energy has a subspace of extremum points, not a single extremum
* Seifert surfaces: understand the logic and probably improve

### Data

* drop the zoo (e.g., the folder "300"), provide good single version for each knot

### Possible future development

* multi-component links 
* combinatorial invariants/params for diagrams
* saving 3D scene in VRML (or what is the standard de facto at the moment?)

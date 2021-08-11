* new operation: remove edge
* save changed caption in diagram file (and the same for knot)
* undo/redo: change editor mode if necessary
* fix "isSaved" computation: probably the best way is to save latest saved version in DiagramEditor and compare current versio with it
* move code from DiagramWidget to DiagramEditor (e.g., method 'canDeleteVertex')

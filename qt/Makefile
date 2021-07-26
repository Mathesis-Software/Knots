include ../config.mk

all:
	make -C setupColor all
	make -C setValue all
	make -C abstractWindow all
	make -C diagramWindow all
	make -C 3dWindow all
	make -C knotWindow all
	make -C manager all

clean:
	make -C setupColor .clean
	make -C setValue .clean
	make -C abstractWindow .clean
	make -C diagramWindow .clean
	make -C 3dWindow .clean
	make -C knotWindow .clean
	make -C manager .clean
	$(RM) manager/KnotEditor

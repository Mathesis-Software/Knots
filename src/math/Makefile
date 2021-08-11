SUBDIRS = diagram diagramEditor knot knot/computables seifert

all:
	@for dir in $(SUBDIRS); do make -C $$dir all; done

clean:
	@for dir in $(SUBDIRS); do make -C $$dir .clean; done

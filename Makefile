ROOTDIR = .

include config.mk

all:
	make -C math all
	make -C gl all
	make -C qt all

install: install_bin install_data install_pixmaps

install_bin: all
	$(INSTALL) -d $(INSTPREFIX)/bin
	./create_script.sh $(INSTDIR) > $(INSTPREFIX)/bin/knoted
	chmod 755 $(INSTPREFIX)/bin/knoted
	$(INSTALL) -d $(INSTDIR)
	$(INSTALL) -d $(INSTDIR)/bin
	$(INSTALL) -s qt/manager/KnotEditor $(INSTDIR)/bin
	
install_data:
	$(INSTALL) -d $(INSTDIR)/data
	$(INSTALL) -d $(INSTDIR)/data/trefoil
	$(INSTALL) -d $(INSTDIR)/data/300
	$(INSTALL) -m 644 data/*.* $(INSTDIR)/data
	$(INSTALL) -m 644 data/trefoil/*.* $(INSTDIR)/data/trefoil
	$(INSTALL) -m 644 data/trefoil/*.* $(INSTDIR)/data/300

install_pixmaps:
	$(INSTALL) -d $(INSTDIR)/pixmaps
	$(INSTALL) -m 644 pixmaps/* $(INSTDIR)/pixmaps

archive:
	ls > .ls
	mkdir KnotEditor-$(VERSION)
	cp -r `cat .ls` KnotEditor-$(VERSION)
	make -C KnotEditor-$(VERSION) distclean
	$(RM) -r KnotEditor-$(VERSION)/KnotEditor-$(VERSION)
	tar czf KnotEditor-$(VERSION).tar.gz KnotEditor-$(VERSION)
	$(RM) -r KnotEditor-$(VERSION) .ls

clean:
	@make -C math clean
	@make -C gl clean
	@make -C qt clean
	@make -C other clean
	@make .clean
	@$(RM) *.tar.gz

distclean: clean
	@$(RM) knoted

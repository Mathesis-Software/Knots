ROOTDIR = .

include src/config.mk

all:
	make -C src/math all
	make -C src/qt all

install: install_bin install_data

install_bin: all
	$(INSTALL) -d $(INSTPREFIX)/bin
	./create_script.sh $(INSTDIR) > $(INSTPREFIX)/bin/knoted
	chmod 755 $(INSTPREFIX)/bin/knoted
	$(INSTALL) -d $(INSTDIR)
	$(INSTALL) -d $(INSTDIR)/bin
	$(INSTALL) -s src/qt/manager/KnotEditor $(INSTDIR)/bin
	
install_data:
	$(INSTALL) -d $(INSTDIR)/data
	$(INSTALL) -d $(INSTDIR)/data/trefoil
	$(INSTALL) -d $(INSTDIR)/data/300
	$(INSTALL) -m 644 data/*.* $(INSTDIR)/data
	$(INSTALL) -m 644 data/trefoil/*.* $(INSTDIR)/data/trefoil
	$(INSTALL) -m 644 data/300/*.* $(INSTDIR)/data/300

archive:
	ls > .ls
	mkdir KnotEditor-$(VERSION)
	cp -r `cat .ls` KnotEditor-$(VERSION)
	make -C KnotEditor-$(VERSION) distclean
	$(RM) -r KnotEditor-$(VERSION)/KnotEditor-$(VERSION)
	tar czf KnotEditor-$(VERSION).tar.gz KnotEditor-$(VERSION)
	$(RM) -r KnotEditor-$(VERSION) .ls

clean:
	@make -C src/math clean
	@make -C src/qt clean
	@make -C src/other clean
	@make .clean
	@$(RM) *.tar.gz

distclean: clean
	@$(RM) knoted

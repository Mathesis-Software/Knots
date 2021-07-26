ROOTDIR = ../..

include $(ROOTDIR)/config.mk

OBJECTS = \
	seifert.o \
	seifert_surface.o \
	seifert_ord.o \
	seifert_list.o

#############################################################################

all: seifert.a

seifert.a: $(OBJECTS)
	$(RM) $@
	$(AR) $@ $^

seifert.o: seifert.cpp seifert.h

seifert_surface.o: seifert_surface.cpp seifert_surface.h seifert.h $(ROOTDIR)/gl/surface/surface.h

seifert_list.o: seifert_list.cpp seifert.h

seifert_ord.o: seifert_ord.cpp seifert.h

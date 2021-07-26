ROOTDIR = ../..

include $(ROOTDIR)/config.mk

OBJECTS = surface_base.o surface_gl.o

#############################################################################

all: surface.a

surface.a: $(OBJECTS)
	$(RM) $@
	$(AR) $@ $^

surface_base.o: surface_base.cpp surface.h

surface_gl.o: surface_gl.cpp surface.h

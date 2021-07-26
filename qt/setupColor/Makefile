ROOTDIR = ../..

include $(ROOTDIR)/config.mk

OBJECTS = setupColor.o setupColor.moc.o

#############################################################################

all: setupColor.a

setupColor.a: $(OBJECTS)
	$(RM) $@
	$(AR) $@ $^

setupColor.o: setupColor.cpp setupColor.h

setupColor.moc.o: setupColor.moc.cpp

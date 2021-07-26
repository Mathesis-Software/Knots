ROOTDIR = ../..

include $(ROOTDIR)/config.mk

OBJECTS = abstractWindow.o abstractWindow.moc.o

#############################################################################

all: abstractWindow.a

abstractWindow.a: $(OBJECTS)
	$(RM) $@
	$(AR) $@ $^

abstractWindow.o: abstractWindow.cpp abstractWindow.h

abstractWindow.moc.o: abstractWindow.moc.cpp

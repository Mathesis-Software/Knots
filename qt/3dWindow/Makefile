ROOTDIR = ../..

include $(ROOTDIR)/config.mk

OBJECTS = \
	3dWindow.o	\
	3dMainWidget.o	\
	3dWindow_rotate.o	\
	3dWindow.moc.o

HEADERS = 3dWindow.h ../abstractWindow/abstractWindow.h

#############################################################################

all: 3dWindow.a

3dWindow.a: $(OBJECTS)
	$(RM) $@
	$(AR) $@ $^

3dWindow.o: 3dWindow.cpp $(HEADERS)

3dWindow_rotate.o: 3dWindow_rotate.cpp $(HEADERS)

3dMainWidget.o: 3dMainWidget.cpp $(HEADERS)

3dWindow.moc.cpp: $(HEADERS)

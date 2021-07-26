ROOTDIR = ../..

include $(ROOTDIR)/config.mk

OBJECTS = vertex.o crossing.o diagram_base.o diagram_vertex.o	\
          diagram_stream.o diagram_cross.o diagram_shift.o	\
	  diagram_simplify.o diagram_order.o

#############################################################################

all: diagram.a

diagram.a: $(OBJECTS)
	$(RM) $@
	$(AR) $@ $^

vertex.o: vertex.cpp vertex.h

crossing.o: crossing.cpp vertex.h

diagram_base.o: diagram_base.cpp diagram.h vertex.h

diagram_vertex.o: diagram_vertex.cpp diagram.h vertex.h

diagram_cross.o: diagram_cross.cpp diagram.h

diagram_shift.o: diagram_shift.cpp diagram.h

diagram_order.o: diagram_order.cpp diagram.h

diagram_simplify.o: diagram_simplify.cpp diagram.h

diagram_stream.o: diagram_stream.cpp diagram.h

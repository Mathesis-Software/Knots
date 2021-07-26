ROOTDIR = ../..

include $(ROOTDIR)/config.mk

OBJECTS = \
	knot_base.o		\
	knot_diagram.o		\
	knot_dist.o		\
	knot_math.o		\
	knot_normalize.o	\
	knot_seifert.o		\
	knot_smooth.o		\
	knot_stream.o		\
	knot_surface.o		\
	knot_vassiliev.o

all: knot.a

knot.a: $(OBJECTS)
	$(RM) $@
	$(AR) $@ $^

knot_base.o: knot_base.cpp knot.h ../headers/parameter.h

knot_diagram.o: knot_diagram.cpp knot.h

knot_dist.o: knot_dist.cpp knot.h

knot_math.o: knot_math.cpp knot.h ../headers/parameter.h

knot_normalize.o: knot_normalize.cpp knot.h

knot_seifert.o: knot_seifert.cpp knot.h

knot_smooth.o: knot_smooth.cpp knot.h

knot_stream.o: knot_stream.cpp knot.h

knot_surface.o: knot_surface.cpp knot.h knot_surface.h $(ROOTDIR)/gl/surface/surface.h

knot_vassiliev.o: knot_vassiliev.cpp knot.h

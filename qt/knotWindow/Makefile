ROOTDIR = ../..

include $(ROOTDIR)/config.mk

OBJECTS =       	 	\
	knotWindow_base.o		\
	knotWindow_colors.o	\
	knotWindow_math.o		\
	knotWindow_menu.o		\
	knotWindow_options.o	\
	knotWindow_param.o	\
	knotWindow_seifert.o	\
	knotWindow_smooth.o	\
	knotWindow_stream.o	\
	knotWindow.moc.o		\
	knotWindow_math.moc.o	\
	knotWindow_param.moc.o

COMMON_HEADERS = knotWindow.h $(ROOTDIR)/math/knot/knot.h $(ROOTDIR)/gl/surface/surface.h ../abstractWindow/abstractWindow.h ../3dWindow/3dWindow.h ../diagramWindow/diagramWindow.h

#############################################################################

all: knotWindow.a

knotWindow.a: $(OBJECTS)
	$(RM) $@
	$(AR) $@ $^

knotWindow_base.o: knotWindow_base.cpp $(COMMON_HEADERS) $(ROOTDIR)/math/seifert/seifert_surface.h

knotWindow_colors.o: knotWindow_colors.cpp ../setupColor/setupColor.h $(COMMON_HEADERS)

knotWindow_math.o: knotWindow_math.cpp knotWindow_math.h $(COMMON_HEADERS)

knotWindow_menu.o: knotWindow_menu.cpp $(COMMON_HEADERS)

knotWindow_options.o: knotWindow_options.cpp $(ROOTDIR)/math/knot/knot_surface.h $(COMMON_HEADERS)

knotWindow_param.o: knotWindow_param.cpp knotWindow_param.h $(COMMON_HEADERS)

knotWindow_seifert.o: knotWindow_seifert.cpp $(COMMON_HEADERS)

knotWindow_seifert.o: knotWindow_seifert.cpp $(COMMON_HEADERS)

knotWindow_smooth.o: knotWindow_smooth.cpp $(COMMON_HEADERS)

knotWindow_stream.o: knotWindow_stream.cpp $(COMMON_HEADERS)

knotWindow.moc.cpp: $(COMMON_HEADERS)

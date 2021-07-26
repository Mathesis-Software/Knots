ROOTDIR = ../..

include $(ROOTDIR)/config.mk

OBJECTS =	\
	setValue_Int.o		\
	setValue_Float.o	\
	setValue_String.o	\
	setValue.moc.o

all: setValue.a

setValue.a: $(OBJECTS)
	$(RM) $@
	$(AR) $@ $^

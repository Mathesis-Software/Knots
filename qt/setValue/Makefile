ROOTDIR = ../..

all: setValue.a

include $(ROOTDIR)/config.mk

OBJECTS += setValue.moc.o

setValue.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'

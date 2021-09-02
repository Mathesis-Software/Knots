TARGETS = vassiliev converter torus dtcode

all: $(TARGETS)

CC = ccache clang++
CFLAGS = -c -O2 -std=gnu++1z
LIBS = -L../ke -lke

.cpp.o:
	@echo -n 'Compiling $@ ...'
	@$(CC) $(CFLAGS) -o$@ $<
	@echo ' OK'

dtcode: dtcode.o ../ke/libke.a
	@echo -n 'Linking $@ ...'
	@$(CC) -o$@ $< $(LIBS)
	@echo ' OK'

vassiliev: vassiliev.o ../ke/libke.a
	@echo -n 'Linking $@ ...'
	@$(CC) -o$@ $< $(LIBS)
	@echo ' OK'

converter: converter.o ../ke/libke.a
	@echo -n 'Linking $@ ...'
	@$(CC) -o$@ $< $(LIBS)
	@echo ' OK'

torus: torus.o ../ke/libke.a
	@echo -n 'Linking $@ ...'
	@$(CC) -o$@ $< $(LIBS)
	@echo ' OK'

distclean: clean

clean:
	@$(RM) $(TARGETS) *.o

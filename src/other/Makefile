TARGETS = vassiliev converter torus

all: $(TARGETS)

CC = clang++
CFLAGS = -O2 -std=gnu++1z
LIBS = -L../ke -lke

vassiliev: vassiliev.cpp ../ke/libke.a
	@echo -n 'Linking $@ ...'
	@$(CC) $(CFLAGS) -o$@ $< $(LIBS)
	@echo ' OK'

converter: converter.cpp ../ke/libke.a
	@echo -n 'Linking $@ ...'
	@$(CC) $(CFLAGS) -o$@ $< $(LIBS)
	@echo ' OK'

torus: torus.cpp ../ke/libke.a
	@echo -n 'Linking $@ ...'
	@$(CC) $(CFLAGS) -o$@ $< $(LIBS)
	@echo ' OK'

distclean: clean

clean:
	@$(RM) $(TARGETS)

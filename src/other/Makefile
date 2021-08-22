include ../config.mk

all: vassiliev converter

vassiliev: vassiliev.cpp ../ke/libke.a
	@echo -n 'Linking $@ ...'
	@$(CC) $(CFLAGS) -o$@ $< -L../ke -lke
	@echo ' OK'

converter: converter.cpp ../ke/libke.a
	@echo -n 'Linking $@ ...'
	@$(CC) $(CFLAGS) -o$@ $< -L../ke -lke
	@echo ' OK'

clean:
	@$(RM) vassiliev converter

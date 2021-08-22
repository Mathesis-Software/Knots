include ../config.mk

all: vassiliev

vassiliev: vassiliev.cpp ../ke/libke.a
	@echo -n 'Linking $@ ...'
	@$(CC) $(CFLAGS) -ovassiliev $< -L../ke -lke
	@echo ' OK'

clean:
	@$(RM) vassiliev

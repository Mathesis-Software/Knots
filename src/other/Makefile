include ../config.mk

all: vassiliev

vassiliev: vassiliev.cpp ../ke/libke.a
	$(CC) -ovassiliev $< -L../ke -lke

clean:
	$(RM) vassiliev

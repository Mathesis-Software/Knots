VERSION = `cat $(ROOTDIR)/VERSION`

#############################################################################

CC = ccache g++
MOC = moc
AR = ar rc
RM = rm -f
INSTALL = install

CFLAGS = -O3 -pipe -fPIC
CFLAGS += -W -Wall -Wno-deprecated-copy

DEBUGOPT =
#DEBUGOPT = -g -pg

CFLAGS += $(DEBUGOPT)

QTINCDIR = /usr/include/x86_64-linux-gnu/qt5
QTLIBDIR = /usr/lib

INSTPREFIX = /usr/local
INSTDIR = $(INSTPREFIX)/KnotEditor-$(VERSION)

LIBS = -L$(QTLIBDIR) -lstdc++ -lQt5Core -lQt5Gui -lQt5PrintSupport -lQt5Widgets -lQt5OpenGL -lGL

#############################################################################

SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

.SUFFIXES: .cpp .h .moc.cpp

.cpp.o:
	@echo -n 'Compiling $@ ...'
	@$(CC) -MM $(CFLAGS) -DVERSION="\"$(VERSION)\"" -I$(QTINCDIR) $< -o `basename $< .cpp`.d
	@$(CC) -c $(CFLAGS) -DVERSION="\"$(VERSION)\"" -I$(QTINCDIR) $<
	@echo ' OK'

.h.moc.cpp:
	@echo -n 'Mocifying $@ ...'
	@$(MOC) $< -o $@
	@echo ' OK'

#############################################################################

.clean:
	@$(RM) *.a *.o *.d *.moc.cpp gmon.out err* tmp* *.sw? .*.sw?

-include *.d

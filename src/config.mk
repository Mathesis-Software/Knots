VERSION = `cat $(ROOTDIR)/VERSION`

#############################################################################

CC = ccache g++
MOC = moc
RC = rcc
AR = ar rc
RM = rm -f
INSTALL = install

CFLAGS = -O3 -pipe -fPIC -std=c++17
CFLAGS += -W -Wall -Wno-deprecated-copy

DEBUGOPT =
#DEBUGOPT = -g -pg

CFLAGS += $(DEBUGOPT)

QTINCDIR = /usr/include/x86_64-linux-gnu/qt5
QTLIBDIR = /usr/lib

INSTPREFIX = /usr/local
INSTDIR = $(INSTPREFIX)/KnotEditor

LIBS = -L$(QTLIBDIR) -lstdc++ -lQt5Core -lQt5Gui -lQt5Widgets -lQt5OpenGL -lGL

#############################################################################

MOC_HEADERS = $(shell fgrep -H -s Q_OBJECT *.h | cut -d : -f 1 | uniq)
MOC_SOURCES = $(patsubst %.h, %.moc.cpp, $(MOC_HEADERS))
RC_FILES = $(wildcard *.qrc)
RC_SOURCES = $(patsubst %.qrc, %.qrc.cpp, $(RC_FILES))
SOURCES = $(wildcard *.cpp) $(MOC_SOURCES) $(RC_SOURCES)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

.PRECIOUS: $(MOC_SOURCES) $(RC_SOURCES)
.SUFFIXES: .cpp .h .moc.cpp .qrc .qrc.cpp

.cpp.o:
	@echo -n 'Compiling $@ ...'
	@$(CC) -MM $(CFLAGS) -DVERSION="\"$(VERSION)\"" -I$(QTINCDIR) $< -o `basename $< .cpp`.d
	@$(CC) -c $(CFLAGS) -DVERSION="\"$(VERSION)\"" -I$(QTINCDIR) $<
	@echo ' OK'

.h.moc.cpp:
	@echo -n 'Mocifying $@ ...'
	@$(MOC) $< -o $@
	@echo ' OK'

.qrc.qrc.cpp:
	@echo -n 'Compiling resources $@ ...'
	@$(RC) -o $@ $<
	@echo ' OK'

#############################################################################

.clean:
	@$(RM) *.a *.o *.d *.moc.cpp *.qrc.cpp gmon.out err* tmp* *.sw? .*.sw?

-include *.d

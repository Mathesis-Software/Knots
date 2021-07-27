VERSION = `cat $(ROOTDIR)/VERSION`

#############################################################################

CC = ccache g++
MOC = moc
AR = ar rc
RM = rm -f
INSTALL = install

CFLAGS = -O3 -pipe -fPIC
CFLAGS += -W -Wall

DEBUGOPT =
#DEBUGOPT = -g -pg

CFLAGS += $(DEBUGOPT)

QTINCDIR = /usr/include/x86_64-linux-gnu/qt5
QTLIBDIR = /usr/lib

INSTPREFIX = /usr/local
INSTDIR = $(INSTPREFIX)/KnotEditor-$(VERSION)

LIBS = -L$(QTLIBDIR) -lstdc++ -lQt5Core -lQt5Gui -lQt5PrintSupport -lQt5Widgets -lQt5OpenGL -lGL

#############################################################################

.SUFFIXES: .cpp .h .moc.cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -DVERSION="\"$(VERSION)\"" -I$(QTINCDIR) $<

.h.moc.cpp:
	$(MOC) $< -o $@

#############################################################################

.clean:
	$(RM) *.a *.o *.moc.cpp gmon.out err* tmp* *.sw? .*.sw?

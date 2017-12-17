# to be included from singlepage/Makefile and multipage/Makefile

PACKAGE=bgc
BASE=../../..
SRCDIR=$(BASE)/src
SRC=$(SRCDIR)/$(PACKAGE).xml
CSS=$(SRCDIR)/$(PACKAGE).css
VALIDFILE=$(BASE)/$(PACKAGE).valid
BINPATH=$(BASE)/bin
LIBPATH=$(BASE)/lib
IMGPATH=$(SRCDIR)/images
IMGS=
HEADER="Beej's Guide to C"

PYTHONPATH=../lib:$(LIBPATH)
export PYTHONPATH


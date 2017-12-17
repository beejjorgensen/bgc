FOP=fop
PACKAGE=bgc

BASE=../..
VALIDFILE=$(BASE)/$(PACKAGE).valid
SRCDIR=$(BASE)/src
SRC=$(SRCDIR)/$(PACKAGE).xml
BINPATH=$(BASE)/bin
LIBPATH=$(BASE)/lib
IMGPATH=$(BASE)/images
IMGS=
#FOPOPTIONS=-c fop.xconf

DESTS=$(PACKAGE)_USLetter.pdf $(PACKAGE)_USLetter_2.pdf \
	$(PACKAGE)_A4.pdf $(PACKAGE)_A4_2.pdf \

#DESTS=$(PACKAGE)_USLetter.pdf $(PACKAGE)_USLetter_2.pdf \
#	$(PACKAGE)_USLetter.ps.gz $(PACKAGE)_USLetter_2.ps.gz \
#	$(PACKAGE)_A4.pdf $(PACKAGE)_A4_2.pdf \
#	$(PACKAGE)_A4.ps.gz $(PACKAGE)_A4_2.ps.gz

PYTHONPATH=../lib:$(LIBPATH)
export PYTHONPATH

.PHONY: all
all: $(VALIDFILE) $(DESTS)

$(VALIDFILE): $(SRC)
	$(BINPATH)/bgvalidate $< $@

# implicit rules -----------

%.pdf: %.fo
	$(FOP) $(FOPOPTIONS) $< -pdf $@

%.ps: %.fo
	$(FOP) $(FOPOPTIONS) $< -ps $@

%.ps.gz: %.ps
	gzip -f $<

# FO -----------------------

$(PACKAGE)_USLetter.fo: $(VALIDFILE)
	( cd $(SRCDIR); ../bin/bg2fo -o ../builders/print/$@ $(PACKAGE).xml )

$(PACKAGE)_USLetter_2.fo: $(VALIDFILE)
	( cd $(SRCDIR); ../bin/bg2fo -2 -o ../builders/print/$@ $(PACKAGE).xml )

$(PACKAGE)_A4.fo: $(VALIDFILE)
	( cd $(SRCDIR); ../bin/bg2fo -a4 -o ../builders/print/$@ $(PACKAGE).xml )

$(PACKAGE)_A4_2.fo: $(VALIDFILE)
	( cd $(SRCDIR); ../bin/bg2fo -a4 -2 -o ../builders/print/$@ $(PACKAGE).xml )

# PDF ----------------------

$(PACKAGE)_USLetter.pdf: $(PACKAGE)_USLetter.fo $(IMGS)
$(PACKAGE)_USLetter_2.pdf: $(PACKAGE)_USLetter_2.fo $(IMGS)
$(PACKAGE)_A4.pdf: $(PACKAGE)_A4.fo $(IMGS)
$(PACKAGE)_A4_2.pdf: $(PACKAGE)_A4_2.fo $(IMGS)

# PS -----------------------

$(PACKAGE)_USLetter.ps: $(PACKAGE)_USLetter.fo $(IMGS)
$(PACKAGE)_USLetter_2.ps: $(PACKAGE)_USLetter_2.fo $(IMGS)
$(PACKAGE)_A4.ps: $(PACKAGE)_A4.fo $(IMGS)
$(PACKAGE)_A4_2.ps: $(PACKAGE)_A4_2.fo $(IMGS)

$(PACKAGE)_USLetter.ps.gz: $(PACKAGE)_USLetter.ps
$(PACKAGE)_USLetter_2.ps.gz: $(PACKAGE)_USLetter_2.ps
$(PACKAGE)_A4.ps.gz: $(PACKAGE)_A4.ps
$(PACKAGE)_A4_2.ps.gz: $(PACKAGE)_A4_2.ps

# IMAGES -------------------
#dataencap-300-4.736.png: $(IMGPATH)/dataencap-300-4.736.png
#	cp $< $@

.PHONY: pristine
pristine: clean
	rm -f *.pdf *.ps *.ps.gz

.PHONY: clean
clean:
	rm -f *.fo *.png


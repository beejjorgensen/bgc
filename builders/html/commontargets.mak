# to be included from singlepage/Makefile and multipage/Makefile

#cs-120-3.334.png: $(IMGPATH)/cs-120-3.334.png
#	cp $< $@

$(VALIDFILE): $(SRC)
	$(BINPATH)/bgvalidate $< $@

.PHONY: pristine
pristine: clean
	rm -f *.html $(IMGS) $(PACKAGE).css
	rm -f ../*.tgz ../*.zip

.PHONY: clean
clean:


PACKAGE=bgc
UPLOADDIR=beej71@alfalfa.dreamhost.com:~/beej.us/guide/$(PACKAGE)
BUILDDIR=./stage
BUILDTMP=./build_tmp

.PHONY: all
all:
	$(MAKE) -C src

.PHONY: stage
stage:
	mkdir -p $(BUILDDIR)/{pdf,html,translations,examples}
	cp -v website/* website/.htaccess $(BUILDDIR)
	cp -v src/$(PACKAGE)*.pdf $(BUILDDIR)/pdf
	cp -v src/$(PACKAGE).html $(BUILDDIR)/html/index.html
	cp -v src/$(PACKAGE)-wide.html $(BUILDDIR)/html/index-wide.html
	#cp -v src/{cs,dataencap}.svg $(BUILDDIR)/html/
	cp -v translations/*.{pdf,html} $(BUILDDIR)/translations 2>/dev/null || : 
	cp -v examples/*.c $(BUILDDIR)/examples
	cp -v examples/{Makefile,README.md} $(BUILDDIR)/examples
	mkdir -p $(BUILDTMP)/$(PACKAGE)_examples
	cp -v examples/{Makefile,README.md} examples/*.c $(BUILDTMP)/$(PACKAGE)_examples
	( cd $(BUILDTMP); zip -r $(PACKAGE)_examples.zip $(PACKAGE)_examples )
	cp -v $(BUILDTMP)/$(PACKAGE)_examples.zip $(BUILDDIR)/examples
	rm -rf $(BUILDTMP)

.PHONY: upload
upload: pristine all stage
	rsync -rv -e ssh --delete $(BUILDDIR)/* $(BUILDDIR)/.htaccess $(UPLOADDIR)

.PHONY: fastupload
fastupload: all stage
	rsync -rv -e ssh --delete $(BUILDDIR)/* $(BUILDDIR)/.htaccess $(UPLOADDIR)

.PHONY: pristine
pristine: clean
	$(MAKE) -C src $@
	$(MAKE) -C examples $@
	rm -rf $(BUILDDIR)

.PHONY: clean
clean:
	rm -rf 
	$(MAKE) -C src $@
	$(MAKE) -C examples $@


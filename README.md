Instructions
==========================================================================
Building:

You'll need:

   o  Gnu make
   o  Python 2.4+
   o  Apache XercesJ (for validation; see step 0, below)
   o  Apache FOP (or hack in some other FO processor for print output)

0) Edit bin/bgvalidate to set the right path for XercesJ, or uncomment
   the "disable=1" line to disable validation.

1) Type "make" from the top-level directory.

   If you have Gnu Make, it should work fine.  Other makes might work as
   well.  Windows users might want to check out Cygwin.

2) There is no step two.

You can also cd to anywhere in the output directory tree and "make".

"make clean" cleans, and "make pristine" cleans to "original" state.

To embed your own fonts in the PDFs, see the file output/print/fop.xconf
for instructions.

The "buildall" script demonstrates the build steps for a complete
release.  You'll need to change the "upload" target in the top-level
Makefile to point to your host if you want to use that.  You're free to
upload whatever versions you desire individually, as well.


TODO
some way to make it more beginner-like, like that MIT thing?

questions for each section 

And then he talks about variables on the stack and the heap.


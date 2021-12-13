# Answers to exercises

Please note that these are _some_ answers, and not _The_ answers.
Programming is a creative endeavor and there are infinitely varied
solutions to problems.

Pick the subdirectory corresponding to your chapter, and type `make` in
that directory to build the solutions.

## Build Notes

The `Makefile`s call `gcc` to build, and are expected to run on a
Unix-like (e.g. Linux, BSD, Mac OSX, Windows WSL).

Linking:

* Math programs are linked with `-lm`.
* Threaded programs are linked with `-lpthread`.
* Atomic programs are linked with `-latomic`.

Not all features are necessarily available on all platforms.
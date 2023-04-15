# Guía de Beej para C

Esta es la fuente (traducida al español) de la Guía de Beej para C.

Si únicamente deseas leer la guía, visite el sitio web [Beej's Guide to C.](https://beej.us/guide/bgc/)

Esto está aquí para que Beej tenga todo (lo referente a la guía) en un sólo repositorio y para que los traductores puedan clonarlo fácilmente.

## Instrucciones de construcción

### Dependencias

- [Gnu make](https://www.gnu.org/software/make/) (XCode make también funciona)
- [Python 3+](https://www.python.org/)
- [Pandoc 2.7.3+](https://pandoc.org/)
- XeLaTeX (se puede encontrar en [TeX Live](https://www.tug.org/texlive/) )
- [Fuentes Liberation](https://en.wikipedia.org/wiki/Liberation_fonts) (sans, serif, mono)

Instalación de dependencias en Mac (es necesario reabrir la terminal después de realizar lo siguiente):

```
xcode-select --install                  # instala make
brew install python                     # instala Python3
brew install pandoc
brew install mactex                     # instala XeLaTeX
brew tap homebrew/cask-fonts
brew install font-liberation            # instala fuentes Liberation
```

Es posible que deba agregar algo como esto al path para encontrar `xelatex` :

```
PATH=$PATH:/usr/local/texlive/2021/bin/universal-darwin
```

### Construir

1. Escriba `make` desde el directorio de nivel superior.

    Si tiene Gnu Make, debería funcionar bien. Otros make  podrían funcionar también. Los usuarios de Windows podrían considerar el uso de Cygwin.

2. Escriba `make stage` para copiar todos los productos de compilación y el sitio web en el directorio `stage` .

3. No hay paso tres.

También puede usar `cd` en el directorio `src` y `make` .

`make clean` limpia  y `make pristine` limpia al estado "original".

Para incrustar sus propias fuentes en los archivos PDF, consulte `src/Makefile` para ver ejemplos.

El objetivo `upload` en el `Makefile` raíz demuestra los pasos de compilación para una versión completa. Deberá cambiar la macro `UPLOADDIR` en el `Makefile` de nivel superior para que apunte a su host si desea usar eso. También puede cargar las versiones que desee de forma individual.

## Pull Requests (contribuciones)

Por favor, manténgalos en la escala de correcciones de errores y errores tipográficos. De esa manera, no tengo que considerar ningún problema de derechos de autor al fusionar cambios.

## Por hacer

- Proyectos
- Más información sobre identificadores reservados
- Digrafos/Trigrafos
- C23

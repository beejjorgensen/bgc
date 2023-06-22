<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->


<!-- No hyphenation -->
[nh[scalbn]]
[nh[scalbnf]]
[nh[scalbnl]]
[nh[scalbln]]
[nh[scalblnf]]
[nh[scalblnl]]
<!-- Can't do things that aren't letters
[nh[atan2]]
[nh[atan2f]]
[nh[atan2l]]
-->
[nh[lrint]]
[nh[lrintf]]
[nh[lrintl]]
[nh[llrint]]
[nh[llrintf]]
[nh[llrintl]]
[nh[lround]]
[nh[lroundf]]
[nh[lroundl]]
[nh[llround]]
[nh[llroundf]]
[nh[llroundl]]

<!-- Index see alsos -->
[is[String==>see `char *`]]
[is[New line==>see `\n` newline]]
[is[Ternary operator==>see `?:` ternary operator]]
[is[Addition operator==>see `+` addition operator]]
[is[Subtraction operator==>see `-` subtraction operator]]
[is[Multiplication operator==>see `*` multiplication operator]]
[is[Division operator==>see `/` division operator]]
[is[Modulus operator==>see `%` modulus operator]]
[is[Boolean NOT==>see `!` operator]]
[is[Boolean AND==>see `&&` operator]]
[is[Boolean OR==>see `||` operator]]
[is[Bell==>see `\a` operator]]
[is[Tab (is better)==>see `\t` operator]]
[is[Carriage return==>see `\r` operator]]
[is[Hexadecimal==>see `0x` hexadecimal]]

# Prefacio

> *C no es un gran lenguaje, y no está bien servido por un gran libro.*
>
> --Brian W. Kernighan, Dennis M. Ritchie

No tiene sentido desperdiciar palabras aquí, amigos, saltemos directamente al código C:

```{.c}
E((ck?main((z?(stat(M,&t)?P+=a+'{'?0:3:
execv(M,k),a=G,i=P,y=G&255,
sprintf(Q,y/'@'-3?A(*L(V(%d+%d)+%d,0)
```

Y vivieron felices para siempre. Fin

¿Qué es esto? ¿Dicen que algo aún no está claro sobre todo este asunto del lenguaje de programación C?

Bueno, para ser honesto, ni siquiera estoy seguro de qué hace el código anterior. Es un fragmento de una de las participaciones en el [fl[Concurso internacional de código C ofuscado|https://www.ioccc.org/]][i[Concurso internacional de código C ofuscado]] de 2001, una maravillosa competencia en la que los participantes intentan escribir el código C más ilegible posible, con resultados a menudo sorprendentes.

La mala noticia es que si eres un principiante en todo esto, ¡todo el código C que veas probablemente parezca ofuscado! La buena noticia es que no va a ser así por mucho tiempo.

Lo que intentaremos hacer a lo largo de esta guía es guiarte desde la completa, total y masiva confusión hasta el tipo de iluminación feliz que sólo se puede obtener a través de pura programación en C

En los viejos tiempos, C era un lenguaje más simple. Un buen número de las características contenidas en este libro y *muchas* de las características en el Libro de Referencias no existían cuando K&amp;R escribieron la famosa segunda edición de su libro en 1988. Sin embargo, el lenguaje sigue siendo pequeño en su esencia, y espero presentarlo aquí de una manera que comience con ese núcleo principal simple y se desarrolle hacia afuera.

Y ése es mi pretexto para escribir un libro tan graciosamente grande para un lenguaje tan pequeño y conciso.

## Público

Esta guía asume que ya tienes algunos conocimientos de programación provenientes de otro lenguaje, como [flw[Python|Python_(programming_language)]], [flw[JavaScript|JavaScript]], [flw[Java|Java_(programming_language)]], [flw[Rust|Rust_(programming_language)]], [flw[Go|Go_(programming_language)]], [flw[Swift|Swift_(programming_language)]], etc.  ¡Los desarrolladores de ([flw[Objective-C|Objective-C]] lo tendrán bastante fácil!))

Vamos a suponer que sabes qué son las variables, qué hacen los bucles, cómo funcionan las funciones, etc.

Si ese no es tu caso por alguna razón, lo mejor que puedo ofrecerte es un entretenimiento honesto para tu placer lector. Lo único que puedo prometer razonablemente es que esta guía no terminará en un suspenso... ¿o *sí* ?

## Cómo leer este libro

La guía está en dos volúmenes, y este es el primero: ¡el volumen del tutorial!

El segundo volumen es [fl[biblioteca de referencia|https://beej.us/guide/bgclr/]], y es mucho más una referencia que un tutorial.

Si eres nuevo, siga la parte del tutorial en orden, en general, cuanto más avanzas en los capítulos, menos importante es ir en orden.

Y sin importar tu nivel de habilidad, la parte de referencia está ahí con ejemplos completos de las llamadas a funciones de librerías estándar para ayudarte a refrescar la memoria cuando sea necesario. Excelente lectura para acompañar un plato de cereal o en otro momento.

Finalmente, al ojear el índice (si estás leyendo la versión impresa), las entradas de la sección de referencia están en cursiva.

## Plataforma y Compilador

Trataré de apegarme al antiguo y clásico  [flw[ISO-standard C|ANSI_C]]. Bueno, la mayor parte. Aqui y alla podré alocarme un poco y empezar a hablar de  [flw[POSIX|POSIX]] o algo así, pero ya veremos

Los usuarios **de Unix** (por ejemplo, Linux, BSD, etc.) intenten ejecutar `cc` o `gcc` desde la línea de comandos; es posible que ya tengan un compilador instalado. Si no, busquen en su distribución cómo instalar `gcc` o `clang` .

Los usuarios **de Windows** deberían consultar [fl[Visual Studio Community|https://visualstudio.microsoft.com/vs/community/]]. O, si está buscando una experiencia más similar a Unix (¡recomendado!), instalar [fl[WSL|https://docs.microsoft.com/en-us/windows/wsl/install-win10]] y `gcc` .

Los usuarios **de Mac** querrán instalar [fl[XCode|https://developer.apple.com/xcode/]] y, en particular, las herramientas de línea de comandos.

Hay muchos compiladores por ahí, y virtualmente todos ellos funcionarán para este libro. Y un compilador de C++ compilará la mayoría (¡pero no todo!) del código C. Es mejor usar un compilador propiamente C de ser posible

## Página Web Oficial

La ubicación oficial de este documento es [fl[https://beej.us/guide/bgc/|https://beej.us/guide/bgc/]]. Tal vez esto cambie en el futuro, pero es más probable que todas las demás guías se migren desde las computadoras de Chico State.

## Política de Email

Por lo general, estoy disponible para ayudar con preguntas por correo electrónico, así que no dudes en escribirme, pero no puedo garantizar una respuesta. Llevo una vida bastante ocupada y hay momentos en los que simplemente no puedo responder a una pregunta que tengas. Cuando es ese el caso, generalmente solo borro el mensaje. No es nada personal; Simplemente nunca tendré tiempo para dar la respuesta detallada que necesitas.

Como regla general, cuanto más compleja es la pregunta, menos probable es que responda. Si puedes reducir tu pregunta antes de enviarla por correo y asegurarte de incluir cualquier información pertinente (como plataforma, compilador, mensajes de error que recibes y cualquier otra cosa que creas que podría ayudarme a solucionar el problema), es mucho más probable que obtengas una respuesta.

Si no obtienes una respuesta, inténtalo un poco más, trata de encontrar la respuesta y, si aún te resulta difícil, escríbeme de nuevo con la información que encontraste y, con suerte, será suficiente para ayudarte.

Ahora que te he molestado acerca de cómo escribir y cómo no escribirme, me gustaría que sepas que aprecio *completamente* todos los elogios que ha recibido la guía a lo largo de los años. ¡Es un verdadero impulso moral y me alegra saber que se está utilizando para bien! `:-)` ¡Gracias!

## Duplicación

Eres más que bienvenido a duplicar este sitio, ya sea pública o privadamente. Si reflejas públicamente el sitio y quieres que lo enlace desde la página principal, escríbeme a [`beej@beej.us`](mailto:beej@beej.us) .

## Nota para traductores

Si deseas traducir la guía a otro idioma, escríbeme a [`beej@beej.us`](beej@beej.us) y enlazaré a tu traducción desde la página principal. Siéntete libre de agregar tu nombre e información de contacto a la traducción.

Ten en cuenta las restricciones de licencia en la sección "Derechos de Autor y Distribución", a continuación.

## Derechos de autor y distribución

La guía de Beej para C tiene derechos de autor © 2021 Brian "Beej Jorgensen" Hall.

Con excepciones específicas para el código fuente y las traducciones, a continuación, este trabajo está bajo la licencia Creative Commons Reconocimiento-No comercial-Sin obras derivadas (by-nc-nd) 3.0. Para ver una copia de esta licencia, visite [`https://creativecommons.org/licenses/by-nc-nd/3.0/`](https://creativecommons.org/licenses/by-nc-nd/3.0/) o envíe una carta a Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, EE. UU. .

Una excepción específica a la parte de la licencia "Sin obras derivadas" es la siguiente: esta guía se puede traducir libremente a cualquier idioma, siempre que la traducción sea precisa y la guía se reimprima en su totalidad. Se aplican las mismas restricciones de licencia a la traducción que a la guía original. La traducción también puede incluir el nombre y la información de contacto del traductor.

El código fuente C presentado en este documento se otorga al dominio público y está completamente libre de cualquier restricción de licencia.

Se alienta libremente a los educadores a recomendar o proporcionar copias de esta guía a sus alumnos.

Póngase en contacto [`beej@beej.us`](beej@beej.us) para obtener más información.

## Dedicatoria

Las cosas más difíciles de escribir esta guía son:

- Aprender el material con suficiente detalle para poder explicarlo.
- Descubrir la mejor manera de explicarlo claramente, un proceso iterativo aparentemente interminable
- Exponerme como una supuesta *autoridad* , cuando en realidad solo soy un ser humano normal que trata de encontrarle sentido a todo, como todos los demás.
- Continuar cuando tantas otras cosas me llaman la atención.

Mucha gente me ha ayudado a través de este proceso, y quiero reconocer a aquellos que han hecho posible este libro.

- Todos los usuarios de Internet que decidieron ayudar a compartir sus conocimientos de una forma u otra. El intercambio gratuito de información instructiva es lo que hace que Internet sea el gran lugar que es.
- Los voluntarios de [fl[cppreference.com|https://en.cppreference.com/]] que proporcionan el puente que lleva de la especificación al mundo real.
- Las personas útiles y conocedoras de [fl[comp.lang.c|https://groups.google.com/g/comp.lang.c]] y [fl[r/C_Programming|https://www.reddit. com/r/C_Programming/]] que me ayudó a superar las partes más difíciles del lenguaje.
- Todos los que enviaron correcciones y  los pedidos de incorporación (pull-requests), desde instrucciones confusas hasta errores tipográficos.

¡Gracias! ♥

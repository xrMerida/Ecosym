#import "@preview/versatile-apa:7.2.0": (
  abstract-page, appendix, appendix-outline, title-page, versatile-apa as apa-style,
)
#import "@preview/versatile-apa:7.2.0": apa-figure

#set document(
  title: [Manual del Simulador de un Ecosistema: Proyecto II],
)

#set text(font: ("TeX Gyre Termes", "Noto Emoji"), lang: "es")

#show: apa-style.with(
  font-size: 12pt,
)

#set heading(numbering: "1.1")

// * * Document Starts Here
#let author = [Xavier R. Mérida]
#title-page(
  authors: (name: author),
  course: [Sección 03: Programación Avanzada (Práctica)],
  instructor: [Ing. Luis P. Ovalle],
  affiliations: [Facultad de Ingeniería: Universidad Rafael Landívar],
  due-date: [04 de noviembre de 2026],
)

#outline()

#let energy = [#emoji.lightning]
#let plant = [#emoji.grapes]
#let herbivore = [#emoji.shoshinsha]
#let carnivore = [#emoji.bone]
#let map_cell = [▣]

#pagebreak()
#title()
En el presente documento se mostrara como utilizar el programa Ecosym, diseñado por #author. El programa entrega una interfaz _REPL_ (_leer-evaluar-imprimir-repetir_ por sus siglas en ingles), un tipo de interfaz commentate vista en simuladores. El programa se presenta en inglés pero no es necesario entender el lenguaje para usarlo.

#linebreak()
Al iniciar el programa, el comando `reset` es ejecutado, comando que también puede ser ejecutado por el usuario. Su función es inicializar una nueva simulación. Refiérase al la sección @reset para saber más de este comando.

= Comandos
== Reset <reset>
El comando `reset` termina y elimina la simulación actual (si hay una) e inicia una nueva simulación. Para ello se hará una #link(<input-int>)[lectura de números enteros]
```
matrix rows [12 - 25]:
matrix columns [12 - 25]:
```
Usted debe ingresar la cantidad de filas (_rows_), y columnas (_columns_) que la simulación tendrán. Refiérase a la @input-int para saber más sobre la entrada de números enteros.
#linebreak()
Posteriormente se le solicitara ingresar la cantidad de cada organismo que usted desee.
```
wolves [5 - 62]:
sheep [5 - 62]:
rabbits [5 - 62]:
plants [5 - 62]:
```
El programa esta diseñado para que sea capaz de generar al menos uno de cada especie.
#linebreak()


#pagebreak()
= Lecturas de `cin` <input>
== Números enteros <input-int>
Algunos comandos le solicitaran ingresar números enteros, para diferenciar la entrada de texto y la de números enteros, se muestra el rango de números entre corchetes
```
insert a number [0 - 100]:
```
en el ejemplo anterior, solamente puede ingresar números mayores o iguales a $0$, o menores o iguales a $100$. A continuación se muestra la forma estándar:
```
etiqueta [mínimo - máximo]: <entrada>
```
Donde la *etiqueta* muestra información de lo que debe escribir, el *mínimo* y el *máximo* el rango permitido y *\<entrada>* donde el usuario escribe.
=== Error de lectura
Si ingresa un numero mayor al máximo o menor al mínimo, o si ingresa texto cuando se le solicita un número se mostrara el siguiente error:
#text(fill: red)[
  ```
  invalid input: number between [0 - 100]
  ```
]
esto indica que el usuario ingreso un número *fuera de rango* o ingreso un *texto*. Se solicitara de nuevo que ingrese un numero entero dentro del rango, este proceso se repetirá hasta que la entrada sea valida, es decir cuando la entrada sea un número y este dentro del rango.

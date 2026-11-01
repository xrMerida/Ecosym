#import "@preview/versatile-apa:7.2.0": (
  abstract-page, appendix, appendix-outline, title-page, versatile-apa as apa-style,
)
#import "@preview/versatile-apa:7.2.0": apa-figure

#set document(
  title: [Manual del Simulador de un Ecosistema: Proyecto II],
)

#set text(font: ("TeX Gyre Termes", "Hermit", "Noto Emoji"), lang: "es")

#show: apa-style.with(
  font-size: 12pt,
)

#set par(first-line-indent: (all: false))

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

#pagebreak()
#title()
En el presente documento se mostrará cómo utilizar el programa Ecosym, diseñado por #author. El programa ofrece una interfaz _REPL_ (_leer-evaluar-imprimir-repetir_, por sus siglas en inglés), una interfaz interactiva común en simuladores. El programa se presenta en inglés, pero no es necesario dominar el idioma para utilizarlo.

#linebreak()
Para ayuda rápida ejecute el comando #link(<help>)[`help`] el cual le muestra información relevante del programa. Al iniciar el programa, el comando #link(<reset>)[`reset`] se ejecuta automáticamente.

= Repl
Al iniciar la simulación se mostrara un _prompt_ (indicador) como el siguiente:
```
ecosym>
```
Los comandos son evaluados al presionar enter, utilize #link(<help>)[`help`] para información de los comandos, si un error es devuelto, refiérase a #link(<input-err-str>)[errores de cadenas de texto] para más información.

= Comandos
== Reiniciar simulación (`reset`) <reset>
El comando `reset` #link(<delete>)[elimina la simulación] actual, si existe, e inicia una nueva simulación. Para ello se solicitarán #link(<input-int>)[números enteros]:
```
matrix x-axis [12 - 25]:
matrix y-axis [12 - 25]:
```
Ingrese el tamaño del mapa. El eje _x_ corresponde a las columnas y el eje _y_ a las filas; ambos valores deben estar entre 12 y 25.
#linebreak()
Después se solicitará la cantidad inicial de cada organismo:
```
wolves [5 - máximo]:
sheep [5 - máximo]:
rabbits [5 - máximo]:
plants [5 - máximo]:
```
El programa reserva espacio para que cada especie pueda recibir al menos cinco organismos. El máximo solamente permite llenar la mitad del mapa.
#linebreak()

== Dibujar el mapa (`draw`) <draw>
El comando `draw` muestra el mapa actual. Las filas se imprimen de arriba hacia abajo y las columnas de izquierda a derecha. Las coordenadas se expresan como `(x, y)`, donde `(0, 0)` es la esquina inferior izquierda:
#grid(
  columns: (1.75fr, 1fr),
  [```
  11  '  '  '  '  '  '  '  '  '  '  '  '
  10  '  '  '  '  '  '  '  '  '  '  '  '
   9  '  '  '  '  '  '  '  '  '  '  '  '
   8  '  '  '  '  '  '  '  '  '  '  '  '
   7  '  '  '  '  '  '  '  '  '  '  '  '
   6  '  '  '  '  '  W  '  '  '  '  '  '
   5  '  '  W  '  R  '  '  '  '  W  '  '
   4  '  '  '  '  '  '  '  '  '  '  '  '
   3  '  '  '  '  '  '  '  '  '  '  '  '
   2  '  W  '  '  '  '  W  '  '  '  '  '
   1  '  '  '  '  '  '  *  '  '  '  '  '
   0  '  '  '  '  '  '  *  '  '  *  *  '
      0  1  2  3  4  5  6  7  8  9 10 11
  ``` ],
  [ #align(left)[#table(
    columns: (auto, auto),
    align: (center, left),
    table.header([*Símbolo*], [*Significado*]),
    [`'`], [celda vacía],
    [`R`], [bayas],
    [`S`], [ovejas],
    [`W`], [lobos],
  )]],
)

== Avanzar la simulación (`step`) <step>
El comando `step` avanza la simulación un turno. En cada turno:

- los animales consumen energía;
- los herbívoros buscan plantas y los carnívoros buscan herbívoros;
- si encuentran una presa dentro de su alcance, la cazan;
- de lo contrario, se acercan a una presa o se mueven a una celda vacía;
- las plantas producen energía y pueden reproducirse en una celda vecina.

Un animal que se queda sin energía muere y se elimina al finalizar el turno. Cada acción realizada se muestra como un mensaje en la consola de la siguiente forma:
```
W move from [3, 4] to [0, 5]
W hunted S at [1, 8]
S died at [1, 8]
W move from [2, 7] to [1, 8]
R move from [8, 9] to [6, 7]
R move from [11, 7] to [9, 5]
R move from [7, 8] to [5, 6]
* was born at [10, 0]
* was born at [6, 0]
```
A diferencia de #link(<delete>)[eliminar una simulación], este comando solamente muestra lo que sucedió en el turno actual, no el historial completo.

== Consultar una celda (`lookup`) <lookup>
El comando `lookup` permite inspeccionar una posición del mapa, se le solicitara ingresar #link(<input-int>)[números enteros]:
```
x-axis [0 - columnas]:
y-axis [0 - filas]:
```
Ingrese primero la coordenada `x` y después la coordenada `y`. Si se encuentra un organismo se mostrara algo como esto:
```
symbol: *
type: plant
energy: 0
location: (5, 10)
```
En caso la celda este vacía se mostrara lo siguiente:
```
empty
```

== Consultar estadísticas (`stats`) <stats>
El comando `stats` muestra la cantidad actual de plantas, herbívoros, carnívoros y el total de organismos actualmente. Por ejemplo:
```
plants: 5
herbivores: 10
carnivores: 5
total: 20
```

== Comandos comunes
=== Ayuda (`help`) <help>
El comando `help` lista los comandos disponibles y su descripción. Por ejemplo:
```
reset	Reset the simulation
draw	Draw the current world
lookup	Inspect an organism at a position
step	Advance the simulation one turn
stats	Show the current statistics of the world
help	Print this help message
clear	Clear the screen
exit	Exit the current repl
```

=== Clear (`clear`)
El comando `clear` limpia la pantalla.

=== Salir (`exit`)
El comando `exit` #link(<delete>)[elimina la simulación] y sale del programa, refiérase a la @delete para más información, esta acción es similar a #link(<reset>)[`reset`].

= Eliminar la simulación <delete>
Al eliminar la simulación, se muestran multiples estadísticas de la simulación, y el historial del más reciente al más antiguo. Por ejemplo:
```
<historial>
--- Current statistics ---
plants: 5
herbivores: 10
carnivores: 5
total: 20

--- Initial statistics ---
plants: 5
herbivores: 10
carnivores: 5
total: 20

== Simulation terminated ==
```

= Entradas de usuario <input>
== Lectura de números enteros <input-int>
Algunos comandos le solicitarán ingresar números enteros. El rango permitido se muestra entre corchetes:
```
insert a number [0 - 100]:
```
En el ejemplo anterior, solamente puede ingresar números mayores o iguales a $0$ y menores o iguales a $100$. La forma general es:
```
etiqueta [mínimo - máximo]: <entrada>
```
La *etiqueta* describe el dato solicitado; el *mínimo* y el *máximo* indican el rango permitido; y *\<entrada>* representa el texto que escribe el usuario.
== Errores de lectura
=== Números enteros
Si ingresa un número mayor al máximo, menor al mínimo o texto cuando se solicita un número, se mostrará un error:
#text(fill: red)[
  ```
  invalid input: number between [0 - 100]
  ```
]
Esto indica que ingresó un número *fuera de rango* o un *carácter no numérico*. Se solicitará nuevamente un número entero dentro del rango. El proceso se repetirá hasta recibir una entrada válida.

=== Cadenas de texto <input-err-str>
La entrada debe escribirse en una línea y confirmarse con `Enter`. Los comandos se escriben exactamente como aparecen en la el comando #link(<help>)[`help`]. Por ejemplo, #link(<step>)[`step`] es válido, pero `Step` no se reconoce y devuelve el siguiente error:
#text(fill: red)[
  ```
  repl: Step: command not found
  ```
]

=== Cerrar la entrada estándar `EOF`
En sistemas Unix, presionar `Ctrl-D` devuelve `EOF` (_final-de-archivo_ por sus siglas en ingles), esto es equivalente a cerrar el descriptor `stdin`, por esa razón el se devuelve un error de entrada como el siguiente y se termina la ejecución con un código de salida 0:
#text(fill: red)[
  ```
  repl: input error
  ```
]

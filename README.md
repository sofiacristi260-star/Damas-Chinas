# Damas-Chinas
Hecho por: Sofía Cristina Gómez Hernández

Lista de los desarrollos innovadores implementados:

Sugerencia de Movimiento
La función analiza todos los movimientos posibles para el jugador actual utilizando el generador de movimientos ya existente.
Luego, aplica un criterio simple de comparación que prioriza las jugadas con mayor número de capturas y, en caso de empate, las rutas más largas.
Finalmente, el sistema muestra por consola la secuencia de posiciones del movimiento más ventajoso.
Esto se logró reutilizando la estructura interna de datos de los movimientos, sin necesidad de modificar las reglas principales del juego.

¿Para que se implementó?
Se implementó para ofrecer una ayuda visual al jugador y facilitarle la toma de decisiones durante la partida. Esta función permite al usuario Obtener una recomendación de ficha que puede mover.

¿Por qué se consideró necesario? 
Normalmente los jugadores nuevos a menudo no saben qué piezas mover al inicio. La sugerencia de movimiento actúa como una guía, mejorando la experiencia del jugador.

¿Cómo se llevó a cabo su implentación?

Deshacer Movimiento 

¿Para qué se implementó?
El objetivo es permitir al jugador revertir su última acción en caso de cometer un error o movimiento accidental. Esto da mayor flexibilidad y contro al usuario durante la partida.

¿Por qué se consideró necesario? 
Al diseñar la mecánica básica de movimiento, se consideró que los jugadores podrían cometer errores al ingresar coordenadas o realizar movimientos no deseados. Por eso, la opción de deshacer se pensó como una herramienta preventiva que brinda flexibilidad y mejora la experiencia de juego al permitir corregir acciones sin reiniciar la partida.

¿Cómo se llevó a cabo su implementación?
Se implementó mediante el registro de cada jugada dentro de un historial.
Cada movimiento almacena los datos esenciales: las coordenadas de origen y destino, las piezas capturadas (si las hay) y el estado del jugador que lo realizó.
Cuando el usuario elige deshacer, el programa toma el último elemento del historial y revierte el tablero, restaurando las piezas eliminadas y regresando el turno al jugador correspondiente.
De esta forma, el sistema mantiene un seguimiento ordenado y reversible de los movimientos realizados.


Para poder compilarlo o ejecutar el juego en Linux hay que: 
1. Abrir una terminal y asegúrarse de tener instalado g++:
sudo apt install g++
2.Luego compilar:
g++ main.cpp BoardEngine.cpp menu.cpp -o damas
Ejecuta:
./damas


Para poder compilarlo o ejecutar el juego en Windows hay que:
1. Asegúrarsee de tener instalado MinGW o MSYS2 con g++.
2. Abrir una terminal en la carpeta del proyecto.
3. Escribir el siguiente comando:
g++ main.cpp BoardEngine.cpp menu.cpp -o damas
4. Ejecuta el juego:
./damas

Para poder compilarlo o ejecutar el juego en Windows hay que:

1. Instalar las herramientas de desarrollo con:
xcode-select --install
2. Compila el programa:
g++ main.cpp BoardEngine.cpp menu.cpp -o damas
3. Ejecuta:
./damas

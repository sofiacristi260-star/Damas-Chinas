#include <iostream>
#include <vector>
#include "damas_internacionales.h"

int main() {
    DamasInternacionales juego;
    juego.iniciarTablero();
    int opcion = -1;
    while (opcion != 6) {
        std::cout << "\nMenú" << std::endl;
        std::cout << "1) Mostrar tablero" << std::endl;
        std::cout << "2) Mover pieza (origen y destino)" << std::endl;
        std::cout << "3) Mover secuencia de capturas" << std::endl;
        std::cout << "4) Deshacer último movimiento" << std::endl;
        std::cout << "5) Sugerir mejor movimiento" << std::endl;
        std::cout << "6) Salir" << std::endl;
        std::cout << "Turno del jugador: " << (juego.obtenerTurno()+1) << std::endl;
        std::cout << "Opción: ";
        std::cin >> opcion;

        if (opcion == 1) {
            juego.mostrarTablero();
        } else if (opcion == 2) {
            int fO, cO, fD, cD;
            std::cout << "Fila origen: ";
            std::cin >> fO;
            std::cout << "Columna origen: ";
            std::cin >> cO;
            std::cout << "Fila destino: ";
            std::cin >> fD;
            std::cout << "Columna destino: ";
            std::cin >> cD;
            bool ok = juego.moverPieza(fO, cO, fD, cD);
            if (ok) std::cout << "Movimiento realizado" << std::endl;
            else std::cout << "Movimiento inválido" << std::endl;
        } else if (opcion == 3) {
            int pasos;
            std::cout << "Cantidad de posiciones en la secuencia: ";
            std::cin >> pasos;
            std::vector<Paso> ruta;
            ruta.reserve(pasos);
            for (int i = 0; i < pasos; ++i) {
                int f, c; std::cout << "Paso " << i+1 << " (fila columna): "; std::cin >> f >> c; ruta.push_back({f,c});
            }
            bool ok = juego.moverSecuencia(ruta);
            if (ok) std::cout << "Secuencia aplicada" << std::endl;
            else std::cout << "Secuencia inválida" << std::endl;
        } else if (opcion == 4) {
            bool ok = juego.deshacerMovimiento();
            if (ok) std::cout << "Último movimiento deshecho" << std::endl;
            else std::cout << "No hay movimientos para deshacer" << std::endl;
        } else if (opcion == 5) {
            juego.sugerirMejorMovimiento();
        } else if (opcion == 6) {
        } else {
            std::cout << "Opción no válida" << std::endl;
        }
    }
    return 0;
}
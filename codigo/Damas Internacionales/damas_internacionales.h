#ifndef DAMAS_INTERNACIONALES_H
#define DAMAS_INTERNACIONALES_H

#include <vector>
#include <utility>
#include <string>

struct Paso {
    int fila;
    int columna;
};

struct CapturaInfo {
    int fila;
    int columna;
    char pieza;
};

struct Movimiento {
    int jugador;
    std::vector<Paso> ruta;
    std::vector<CapturaInfo> capturas;
    bool coronacion;
};

class DamasInternacionales {
public:
    DamasInternacionales();
    void iniciarTablero();
    void mostrarTablero() const;
    int obtenerTurno() const;
    bool moverPieza(int filaOrigen, int columnaOrigen, int filaDestino, int columnaDestino);
    bool moverSecuencia(const std::vector<Paso>& ruta);
    bool deshacerMovimiento();
    void sugerirMejorMovimiento() const;
    bool hayCapturaDisponible(int jugador) const;
private:
    int filas;
    int columnas;
    std::vector<std::vector<char>> tablero;
    int turnoJugador;
    bool enCadena;
    int filaCadena;
    int columnaCadena;
    std::vector<Movimiento> historial;

    bool posicionValida(int f, int c) const;
    bool esEnemigo(int jugador, char pieza) const;
    bool esPropia(int jugador, char pieza) const;
    bool esCoronada(char pieza) const;
    bool puedeMoverHacia(int jugador, int fO, int cO, int fD, int cD) const;
    bool puedeCapturarPaso(int jugador, int fO, int cO, int fD, int cD, CapturaInfo& captura) const;
    void coronarSiAplica(int jugador, int f, int c);
    void cambiarTurno();

    void generarMovimientos(int jugador, std::vector<Movimiento>& movimientos) const;
    void generarCapturasDesde(int jugador, int f, int c, std::vector<Movimiento>& destinos, std::vector<CapturaInfo> capturasPrevias, std::vector<std::vector<bool>> visitado) const;
    void generarPasosDesde(int jugador, int f, int c, std::vector<Movimiento>& destinos) const;
};

#endif
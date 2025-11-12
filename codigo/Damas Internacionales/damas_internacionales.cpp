#include "damas_internacionales.h"
#include <iostream>
#include <algorithm>

DamasInternacionales::DamasInternacionales() : filas(10), columnas(10), turnoJugador(0), enCadena(false), filaCadena(-1), columnaCadena(-1) {
    tablero.assign(filas, std::vector<char>(columnas, '.'));
}

void DamasInternacionales::iniciarTablero() {
    for (int f = 0; f < filas; ++f)
        for (int c = 0; c < columnas; ++c)
            tablero[f][c] = '.';
    for (int f = 0; f < 4; ++f)
        for (int c = 0; c < columnas; ++c)
            if ((f + c) % 2 == 1) tablero[f][c] = 'b';
    for (int f = filas - 4; f < filas; ++f)
        for (int c = 0; c < columnas; ++c)
            if ((f + c) % 2 == 1) tablero[f][c] = 'a';
    turnoJugador = 0;
    enCadena = false;
    historial.clear();
}

void DamasInternacionales::mostrarTablero() const {
    std::cout << "   ";
    for (int c = 0; c < columnas; ++c) std::cout << (c % 10);
    std::cout << "\n";
    for (int f = 0; f < filas; ++f) {
        std::cout << (f < 10 ? " " : "") << f << " ";
        for (int c = 0; c < columnas; ++c) {
            std::cout << tablero[f][c];
        }
        std::cout << "\n";
    }
}

int DamasInternacionales::obtenerTurno() const { return turnoJugador; }

bool DamasInternacionales::posicionValida(int f, int c) const {
    return f >= 0 && f < filas && c >= 0 && c < columnas;
}

bool DamasInternacionales::esCoronada(char pieza) const { return pieza == 'A' || pieza == 'B'; }
bool DamasInternacionales::esPropia(int jugador, char pieza) const { return (jugador == 0 ? (pieza == 'a' || pieza == 'A') : (pieza == 'b' || pieza == 'B')); }
bool DamasInternacionales::esEnemigo(int jugador, char pieza) const { return (jugador == 0 ? (pieza == 'b' || pieza == 'B') : (pieza == 'a' || pieza == 'A')); }

bool DamasInternacionales::puedeMoverHacia(int jugador, int fO, int cO, int fD, int cD) const {
    if (!posicionValida(fD, cD)) return false;
    if (tablero[fD][cD] != '.') return false;
    char p = tablero[fO][cO];
    if (!esPropia(jugador, p)) return false;
    if (esCoronada(p)) {
        int df = fD - fO; int dc = cD - cO;
        if (std::abs(df) == std::abs(dc) && std::abs(df) >= 1) {
            int sf = (df > 0 ? 1 : -1); int sc = (dc > 0 ? 1 : -1);
            int f = fO + sf, c = cO + sc;
            while (f != fD || c != cD) { if (tablero[f][c] != '.') return false; f += sf; c += sc; }
            return true;
        }
        return false;
    } else {
        int df = fD - fO; int dc = cD - cO;
        if (std::abs(dc) == 1) {
            if (jugador == 0 && df == -1) return true;
            if (jugador == 1 && df == 1) return true;
        }
        return false;
    }
}

bool DamasInternacionales::puedeCapturarPaso(int jugador, int fO, int cO, int fD, int cD, CapturaInfo& captura) const {
    if (!posicionValida(fD, cD)) return false;
    if (tablero[fD][cD] != '.') return false;
    char p = tablero[fO][cO];
    if (!esPropia(jugador, p)) return false;
    if (esCoronada(p)) {
        int df = fD - fO; int dc = cD - cO;
        if (std::abs(df) == std::abs(dc) && std::abs(df) >= 2) {
            int sf = (df > 0 ? 1 : -1); int sc = (dc > 0 ? 1 : -1);
            int f = fO + sf, c = cO + sc; bool encontrado = false; int fe = -1, ce = -1; char pe = '.';
            while (f != fD || c != cD) {
                if (tablero[f][c] != '.') {
                    if (encontrado) return false;
                    if (!esEnemigo(jugador, tablero[f][c])) return false;
                    encontrado = true; fe = f; ce = c; pe = tablero[f][c];
                }
                f += sf; c += sc;
            }
            if (encontrado) { captura = {fe, ce, pe}; return true; }
        }
        return false;
    } else {
        int df = fD - fO; int dc = cD - cO;
        if (std::abs(df) == 2 && std::abs(dc) == 2) {
            int fm = fO + df/2, cm = cO + dc/2;
            if (posicionValida(fm, cm) && esEnemigo(jugador, tablero[fm][cm])) {
                if (jugador == 0 && df == -2) { captura = {fm, cm, tablero[fm][cm]}; return true; }
                if (jugador == 1 && df == 2)  { captura = {fm, cm, tablero[fm][cm]}; return true; }
            }
        }
        return false;
    }
}

void DamasInternacionales::coronarSiAplica(int jugador, int f, int c) {
    char p = tablero[f][c];
    if (esCoronada(p)) return;
    if (jugador == 0 && f == 0 && p == 'a') tablero[f][c] = 'A';
    if (jugador == 1 && f == filas-1 && p == 'b') tablero[f][c] = 'B';
}

bool DamasInternacionales::hayCapturaDisponible(int jugador) const {
    for (int f = 0; f < filas; ++f) for (int c = 0; c < columnas; ++c) {
        char p = tablero[f][c]; if (!esPropia(jugador, p)) continue;
        CapturaInfo cap;
        for (int df = -2; df <= 2; ++df) for (int dc = -2; dc <= 2; ++dc) {
            if (df == 0 && dc == 0) continue;
            int fD = f + df, cD = c + dc;
            if (puedeCapturarPaso(jugador, f, c, fD, cD, cap)) return true;
        }
        if (esCoronada(p)) {
            for (int k = 3; k < std::max(filas, columnas); ++k) {
                for (int s1 : {-1,1}) for (int s2 : {-1,1}) {
                    int fD = f + s1*k, cD = c + s2*k;
                    if (posicionValida(fD, cD) && tablero[fD][cD] == '.') {
                        if (puedeCapturarPaso(jugador, f, c, fD, cD, cap)) return true;
                    }
                }
            }
        }
    }
    return false;
}

void DamasInternacionales::generarPasosDesde(int jugador, int f, int c, std::vector<Movimiento>& destinos) const {
    char p = tablero[f][c]; if (!esPropia(jugador, p)) return;
    if (esCoronada(p)) {
        for (int s1 : {-1,1}) for (int s2 : {-1,1}) {
            int fD = f + s1, cD = c + s2;
            while (posicionValida(fD, cD) && tablero[fD][cD] == '.') {
                Movimiento m; m.jugador = jugador; m.ruta = {{f,c},{fD,cD}}; m.coronacion = false;
                destinos.push_back(m);
                fD += s1; cD += s2;
            }
        }
    } else {
        int df = (jugador == 0 ? -1 : 1);
        for (int dc : {-1,1}) {
            int fD = f + df, cD = c + dc;
            if (posicionValida(fD, cD) && tablero[fD][cD] == '.') {
                Movimiento m; m.jugador = jugador; m.ruta = {{f,c},{fD,cD}}; m.coronacion = false;
                destinos.push_back(m);
            }
        }
    }
}

void DamasInternacionales::generarCapturasDesde(int jugador, int f, int c, std::vector<Movimiento>& destinos, std::vector<CapturaInfo> capturasPrevias, std::vector<std::vector<bool>> visitado) const {
    char p = tablero[f][c]; if (!esPropia(jugador, p)) return;
    bool hubo = false;
    if (!esCoronada(p)) {
        for (int dc : {-1,1}) {
            int fM = f + (jugador==0?-1:1), cM = c + dc;
            int fD = f + (jugador==0?-2:2), cD = c + 2*dc;
            CapturaInfo cap;
            if (posicionValida(fM,cM) && !visitado[fM][cM] && posicionValida(fD,cD) && tablero[fD][cD]=='.' && esEnemigo(jugador, tablero[fM][cM])) {
                hubo = true;
                visitado[fM][cM] = true;
                auto nuevas = capturasPrevias; nuevas.push_back({fM,cM,tablero[fM][cM]});
                std::vector<Movimiento> continuaciones;
                generarCapturasDesde(jugador, fD, cD, continuaciones, nuevas, visitado);
                if (continuaciones.empty()) {
                    Movimiento m; m.jugador = jugador; m.ruta = {{f,c},{fD,cD}}; m.capturas = nuevas; m.coronacion = false; destinos.push_back(m);
                } else {
                    for (auto &mm : continuaciones) {
                        Movimiento m; m.jugador = jugador; m.ruta = {{f,c}}; m.ruta.insert(m.ruta.end(), mm.ruta.begin(), mm.ruta.end()); m.capturas = mm.capturas; m.coronacion = false; destinos.push_back(m);
                    }
                }
                visitado[fM][cM] = false;
            }
        }
    } else {
        for (int s1 : {-1,1}) for (int s2 : {-1,1}) {
            int fScan = f + s1, cScan = c + s2; bool encontrado = false; int fe=-1, ce=-1; char pe='.';
            while (posicionValida(fScan, cScan) && tablero[fScan][cScan] == '.') { fScan += s1; cScan += s2; }
            if (posicionValida(fScan, cScan) && esEnemigo(jugador, tablero[fScan][cScan]) && !visitado[fScan][cScan]) {
                encontrado = true; fe = fScan; ce = cScan; pe = tablero[fe][ce];
                int fD = fe + s1, cD = ce + s2;
                while (posicionValida(fD, cD) && tablero[fD][cD] == '.') {
                    hubo = true;
                    visitado[fe][ce] = true;
                    auto nuevas = capturasPrevias; nuevas.push_back({fe,ce,pe});
                    std::vector<Movimiento> continuaciones;
                    generarCapturasDesde(jugador, fD, cD, continuaciones, nuevas, visitado);
                    if (continuaciones.empty()) {
                        Movimiento m; m.jugador = jugador; m.ruta = {{f,c},{fD,cD}}; m.capturas = nuevas; m.coronacion = false; destinos.push_back(m);
                    } else {
                        for (auto &mm : continuaciones) {
                            Movimiento m; m.jugador = jugador; m.ruta = {{f,c}}; m.ruta.insert(m.ruta.end(), mm.ruta.begin(), mm.ruta.end()); m.capturas = mm.capturas; m.coronacion = false; destinos.push_back(m);
                        }
                    }
                    visitado[fe][ce] = false;
                    fD += s1; cD += s2;
                }
            }
        }
    }
}

void DamasInternacionales::generarMovimientos(int jugador, std::vector<Movimiento>& movimientos) const {
    movimientos.clear();
    std::vector<Movimiento> capturas;
    for (int f = 0; f < filas; ++f) for (int c = 0; c < columnas; ++c) {
        if (!esPropia(jugador, tablero[f][c])) continue;
        std::vector<std::vector<bool>> visitado(filas, std::vector<bool>(columnas,false));
        generarCapturasDesde(jugador, f, c, capturas, {}, visitado);
    }
    if (!capturas.empty()) { movimientos = capturas; return; }
    for (int f = 0; f < filas; ++f) for (int c = 0; c < columnas; ++c) {
        generarPasosDesde(jugador, f, c, movimientos);
    }
}

bool DamasInternacionales::moverPieza(int fO, int cO, int fD, int cD) {
    if (!posicionValida(fO,cO) || !posicionValida(fD,cD)) return false;
    if (!esPropia(turnoJugador, tablero[fO][cO])) return false;
    std::vector<Movimiento> movimientos;
    generarMovimientos(turnoJugador, movimientos);
    if (enCadena) {
        std::vector<Movimiento> filtrados;
        for (auto &m : movimientos) if (m.ruta.front().fila == filaCadena && m.ruta.front().columna == columnaCadena) filtrados.push_back(m);
        movimientos.swap(filtrados);
    }
    for (auto &m : movimientos) {
        if (m.ruta.size() == 2 && m.ruta.front().fila == fO && m.ruta.front().columna == cO && m.ruta.back().fila == fD && m.ruta.back().columna == cD) {
            char pieza = tablero[fO][cO];
            tablero[fO][cO] = '.';
            tablero[fD][cD] = pieza;
            for (auto &cap : m.capturas) tablero[cap.fila][cap.columna] = '.';
            coronarSiAplica(turnoJugador, fD, cD);
            bool hayMas = false;
            std::vector<Movimiento> continuaciones;
            std::vector<std::vector<bool>> visitado(filas, std::vector<bool>(columnas,false));
            generarCapturasDesde(turnoJugador, fD, cD, continuaciones, {}, visitado);
            if (!m.capturas.empty() && !continuaciones.empty()) { enCadena = true; filaCadena = fD; columnaCadena = cD; hayMas = true; }
            else { enCadena = false; cambiarTurno(); }
            m.jugador = turnoJugador;
            historial.push_back(m);
            return true;
        }
    }
    return false;
}

bool DamasInternacionales::moverSecuencia(const std::vector<Paso>& ruta) {
    if (ruta.size() < 2) return false;
    int fO = ruta.front().fila, cO = ruta.front().columna;
    if (!esPropia(turnoJugador, tablero[fO][cO])) return false;
    std::vector<Movimiento> movimientos;
    generarMovimientos(turnoJugador, movimientos);
    if (enCadena) {
        std::vector<Movimiento> filtrados;
        for (auto &m : movimientos) if (m.ruta.front().fila == filaCadena && m.ruta.front().columna == columnaCadena) filtrados.push_back(m);
        movimientos.swap(filtrados);
    }
    for (auto &m : movimientos) {
        if (m.capturas.empty()) continue;
        if (m.ruta.size() != ruta.size()) continue;
        bool igual = true;
        for (size_t i = 0; i < ruta.size(); ++i) {
            if (m.ruta[i].fila != ruta[i].fila || m.ruta[i].columna != ruta[i].columna) { igual = false; break; }
        }
        if (!igual) continue;
        char pieza = tablero[fO][cO];
        tablero[fO][cO] = '.';
        int fD = ruta.back().fila, cD = ruta.back().columna;
        tablero[fD][cD] = pieza;
        for (auto &cap : m.capturas) tablero[cap.fila][cap.columna] = '.';
        coronarSiAplica(turnoJugador, fD, cD);
        enCadena = false;
        cambiarTurno();
        historial.push_back(m);
        return true;
    }
    return false;
}

bool DamasInternacionales::deshacerMovimiento() {
    if (historial.empty()) return false;
    Movimiento m = historial.back(); historial.pop_back();
    int fO = m.ruta.front().fila, cO = m.ruta.front().columna;
    int fD = m.ruta.back().fila, cD = m.ruta.back().columna;
    char pieza = tablero[fD][cD];
    tablero[fD][cD] = '.';
    tablero[fO][cO] = (m.coronacion ? (m.jugador==0?'a':'b') : pieza);
    for (auto &cap : m.capturas) tablero[cap.fila][cap.columna] = cap.pieza;
    enCadena = false;
    turnoJugador = m.jugador;
    return true;
}

void DamasInternacionales::cambiarTurno() { turnoJugador = 1 - turnoJugador; }

void DamasInternacionales::sugerirMejorMovimiento() const {
    std::vector<Movimiento> movimientos; generarMovimientos(turnoJugador, movimientos);
    if (movimientos.empty()) { std::cout << "Sin movimientos disponibles" << std::endl; return; }
    auto mejor = *std::max_element(movimientos.begin(), movimientos.end(), [](const Movimiento& a, const Movimiento& b){
        if (a.capturas.size() != b.capturas.size()) return a.capturas.size() < b.capturas.size();
        return a.ruta.size() < b.ruta.size();
    });
    std::cout << "Mejor movimiento: ";
    for (size_t i = 0; i < mejor.ruta.size(); ++i) {
        std::cout << "(" << mejor.ruta[i].fila << "," << mejor.ruta[i].columna << ")";
        if (i + 1 < mejor.ruta.size()) std::cout << " -> ";
    }
    if (!mejor.capturas.empty()) std::cout << "  capturas: " << mejor.capturas.size();
    std::cout << std::endl;
}
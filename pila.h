#ifndef PILA_H
#define PILA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_PALABRA 50

typedef char tString[MAX_PALABRA];

typedef struct {
    tString letrasAdivinadas;
    int intentosRestantes;
    tString palabraMostrada;
} tEstadoJuego; // Registro para modelar un intento del juego con sus letras adivinadas y para mostrar la palabra

typedef struct nodoPila {
    tEstadoJuego estado;
    struct nodoPila* siguiente;
} tNodoPila; // Tipo de dato registro para guardar el estado y autoreferenciar al estado siguiente

typedef tNodoPila* tPila;

void inicializarPila(tPila*);
bool pilaVacia(tPila);
void apilarEstado(tPila*, tEstadoJuego);
bool desapilarEstado(tPila*, tEstadoJuego*);
void liberarPila(tPila*);


void inicializarPila(tPila* pPila) {
    *pPila = NULL;
}

bool pilaVacia(tPila pPila) {
    return pPila == NULL;
}

void apilarEstado(tPila* pPila, tEstadoJuego pEstado) { // Agregamos un nuevo estado por la cima de la pila
    tNodoPila* nuevoNodo = (tNodoPila*)malloc(sizeof(tNodoPila));
    nuevoNodo->estado = pEstado;
    nuevoNodo->siguiente = *pPila; // Agregamos el estado actual del juego en la cima de la pila
    *pPila = nuevoNodo;
}

bool desapilarEstado(tPila* pPila, tEstadoJuego* pEstado) { /* Cuando el jugador elige deshacer el intento con el "."
                                                               se desapila el ultimo estado*/
    if (pilaVacia(*pPila)) return false;
    
    tNodoPila* nodoSuprimir = *pPila;
    
    *pEstado = nodoSuprimir->estado;
    *pPila = (*pPila)->siguiente;
    
    free(nodoSuprimir);
    return true;
}

void liberarPila(tPila* pPila) { // Al terminar una ronda, se borra el historial de movimientos de la palabra
    tEstadoJuego estadoTemp;
    while(desapilarEstado(pPila, &estadoTemp));
}

#endif


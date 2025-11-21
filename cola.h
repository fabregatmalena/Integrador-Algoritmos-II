#ifndef COLA_H
#define COLA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PALABRA 50

#define NOMBRE_ARCHIVO "palabras.txt" 

typedef char tString[MAX_PALABRA];

typedef struct {
    tString palabra;
} tDatoCola;

typedef struct nodoCola {
    tDatoCola dato;
    struct nodoCola* siguiente;
} tNodoCola;

typedef struct {
    tNodoCola* principio;
    tNodoCola* final;
} tColaPalabras;

void inicializarCola(tColaPalabras*);
bool colaVacia(tColaPalabras);
void encolarPalabra(tColaPalabras*, tString);
bool desencolarPalabra(tColaPalabras*, tString);
void cargarPalabrasEnCola(tColaPalabras*);


void inicializarCola(tColaPalabras* pCola) {
    pCola->principio = NULL;
    pCola->final = NULL;
}

bool colaVacia(tColaPalabras pCola) {
    return (pCola.principio == NULL);
}

void encolarPalabra(tColaPalabras* pCola, tString pPalabra) { // Agrega una palabra nueva al final de la cola
    tNodoCola* nuevoNodo = (tNodoCola*)malloc(sizeof(tNodoCola));
    strcpy(nuevoNodo->dato.palabra, pPalabra);
    nuevoNodo->siguiente = NULL;
    if (colaVacia(*pCola)) {
        pCola->principio = nuevoNodo;
        pCola->final = nuevoNodo;
    } else {
        pCola->final->siguiente = nuevoNodo;
        pCola->final = nuevoNodo;
    }
}

bool desencolarPalabra(tColaPalabras* pCola, tString pPalabra) { // Quita la palabra que lleva mas tiempo en la cola, es decir la primera
    if (colaVacia(*pCola)) return false;
    tNodoCola* nodoSuprimir = pCola->principio;
    strcpy(pPalabra, nodoSuprimir->dato.palabra);
    if (pCola->principio == pCola->final) {
        inicializarCola(pCola);
    } else {
        pCola->principio = nodoSuprimir->siguiente;
    }
    free(nodoSuprimir);
    return true;
}

void cargarPalabrasEnCola(tColaPalabras* pCola) { // Lee el archivo y forma una cola de espera con las palabras ordenadas
    FILE* archivo = fopen(NOMBRE_ARCHIVO, "r"); 
    tString palabraLeida;

    if (archivo == NULL) {
        perror("Error al abrir el archivo de palabras");
        printf("Asegurate de que el archivo '%s' exista en la misma carpeta que el ejecutable.\n", NOMBRE_ARCHIVO);
        exit(1); 
    }

    while (fscanf(archivo, "%s", palabraLeida) == 1) {
        encolarPalabra(pCola, palabraLeida);
    }

    fclose(archivo); 
}

#endif

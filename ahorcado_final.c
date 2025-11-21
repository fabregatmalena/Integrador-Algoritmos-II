#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

#include "cola.h" 
#include "pila.h" 

#define MAX_INTENTOS 6

typedef struct {
    tString palabraSecreta;
    tEstadoJuego estadoActual;
} tJuego;

void inicializar(tJuego*);
void procesar(tJuego*);
void finalizar();
void iniciarNuevoRound(tJuego*);
void actualizarPalabraMostrada(tJuego*);
void dibujarAhorcado(int);
void procesarIntento(tJuego*, char);
bool juegoGanado(tJuego);


tColaPalabras colaDePalabras;
tPila pilaUndo;

int main() {
    tJuego juego;
    inicializar(&juego);
    procesar(&juego);
    finalizar();
    
    return 0;
}

void inicializar(tJuego* pJuego) { /* Carga las palabras del archivo en la cola y prepara la pila vacia para deshacer*/
    srand(time(NULL));
    
    inicializarCola(&colaDePalabras);
    inicializarPila(&pilaUndo);
    
    cargarPalabrasEnCola(&colaDePalabras);
    
    printf("Juego del Ahorcado inicializado.\n");
    
    iniciarNuevoRound(pJuego);
}

void procesar(tJuego* pJuego) { /* Mientras la condicion de cumpla, se pide al usuario que ingrese una letra o punto (deshacer)
                                   si se acaban las palabras en la cola, se deja de ejecutar y termina el juego */
    char letra;

    
    while (true) {
        while (pJuego->estadoActual.intentosRestantes > 0 && !juegoGanado(*pJuego)) {
            system("clear || cls");
            printf("--- JUEGO DEL AHORCADO ---\n\n");
            
            dibujarAhorcado(pJuego->estadoActual.intentosRestantes);
            
            printf("\nPalabra: %s\n", pJuego->estadoActual.palabraMostrada);
            printf("Letras ya usadas: %s\n", pJuego->estadoActual.letrasAdivinadas);
            printf("\nControles: [A-Z] Adivinar | [.] Deshacer\n");
            printf("Ingresa una letra (minuscula): ");
            letra = getch();
            letra = tolower (letra);
            
            if (letra >= 'a' && letra <= 'z') {
                procesarIntento(pJuego, letra);
            } else if (letra == '.') {
                if (!desapilarEstado(&pilaUndo, &pJuego->estadoActual)) {
                    printf("\nNo hay movimientos para deshacer.\n");
                    getch();
                }
            }
            
        }

        system("clear || cls");
        
        if (juegoGanado(*pJuego)) { // Si se adivina la palabra antes de los 6 intentos fallidos, se gana el juego
            printf("\n¡FELICIDADES! Has adivinado la palabra: %s\n", pJuego->palabraSecreta);
        } else {
            dibujarAhorcado(0);
            
            printf("\n¡Oh no! Te has quedado sin intentos.\n");
            printf("La palabra secreta era: %s\n", pJuego->palabraSecreta);
        }

        printf("\nPresiona una tecla para el siguiente round...");
        getch();

        if (!colaVacia(colaDePalabras)) { // Mientras hayan palabras en la cola, y se gane el juego, se inicia el siguiente round
            iniciarNuevoRound(pJuego);
        } else {
            break;
        }
    }
}

void finalizar() {
    printf("\n\n--- FIN DEL JUEGO ---\n");
    printf("No hay mas palabras en la cola. ¡Gracias por jugar!\n");
}

void iniciarNuevoRound(tJuego* pJuego) { // Prepara el siguiente nivel sacando una nueva palabra de la cola
    if (!desencolarPalabra(&colaDePalabras, pJuego->palabraSecreta)) {
        printf("Error: No se pudo obtener una palabra de la cola.\n");
        exit(1);
    }
    
    liberarPila(&pilaUndo);
    
    strcpy(pJuego->estadoActual.letrasAdivinadas, "");
    pJuego->estadoActual.intentosRestantes = MAX_INTENTOS;
    
    actualizarPalabraMostrada(pJuego);
}

void actualizarPalabraMostrada(tJuego* pJuego) { // Se encarga de la interfaz grafica de texto y revisa que letra se adivinó
    int i, longitud = strlen(pJuego->palabraSecreta);
    
    for (i = 0; i < longitud; i++) {
        char letraActual = pJuego->palabraSecreta[i];
        
        if (strchr(pJuego->estadoActual.letrasAdivinadas, letraActual) != NULL) {
            pJuego->estadoActual.palabraMostrada[i] = letraActual;
        } else {
            pJuego->estadoActual.palabraMostrada[i] = '_';
        }
    }
    pJuego->estadoActual.palabraMostrada[i] = '\0';
}

void dibujarAhorcado(int pIntentos) { // Dibuja el muñeco según las vidas restantes
    printf("  +---+\n");
    printf("  |   |\n");
    printf("  |   %s\n", (pIntentos < 6) ? "O" : "");
    printf("  |  %s%s%s\n", (pIntentos < 4) ? "/" : " ", (pIntentos < 5) ? "|" : "", (pIntentos < 3) ? "\\" : "");
    printf("  |  %s %s\n", (pIntentos < 2) ? "/" : "", (pIntentos < 1) ? "\\" : "");
    printf("  |\n");
    printf("=========\n");
}

void procesarIntento(tJuego* pJuego, char pLetra) {  
    if (strchr(pJuego->estadoActual.letrasAdivinadas, pLetra) != NULL) { // Verifica si la letra se usó
        printf("\nYa has ingresado esa letra. Intenta con otra.\n");
        getch();
        return;
    }
    apilarEstado( &pilaUndo, pJuego->estadoActual );
    
    int len = strlen(pJuego->estadoActual.letrasAdivinadas); 
    
    pJuego->estadoActual.letrasAdivinadas[len] = pLetra; // Guarda el estado del juego para que el boton de deshacer funcione después
    pJuego->estadoActual.letrasAdivinadas[len + 1] = '\0';
    
    if (strchr (pJuego->palabraSecreta, pLetra) == NULL) { // Aplica una jugada, si acertas revela una letra, sino quita una vida
        (pJuego->estadoActual.intentosRestantes)--;
    } else {
        actualizarPalabraMostrada(pJuego);
    }
}

bool juegoGanado(tJuego pJuego) { /* Verifica si la palabra en pantalla es igual a la palabra secreta, si devuelve true, 
                                     el bucle principal te felicita y pasa a la siguiente ronda */
    return strcmp(pJuego.estadoActual.palabraMostrada, pJuego.palabraSecreta) == 0;
}


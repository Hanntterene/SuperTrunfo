#include <stdio.h>
#include <string.h>
#include "cartas.h"


void exibeCartas (Cartas *lista, int i) {
    if (lista[i].super_trunfo == 1) {
        printf("SUPER TRUNFO\n");
        printf("Carta %d:\n", i + 1);
        printf("  Nome: %s\n", lista[i].nome);
        printf("  Atributos: %d, %d, %d, %d\n", lista[i].a1, lista[i].estrategia, lista[i].popularidade, lista[i].legado);
        printf("  Código: %c-%d\n\n", lista[i].letra, lista[i].numero);
    }
    else {
        printf("Carta %d:\n", i + 1);
        printf("  Nome: %s\n", lista[i].nome);
        printf("  Atributos: %d, %d, %d, %d\n", lista[i].a1, lista[i].estrategia, lista[i].popularidade, lista[i].legado);
        printf("  Código: %c-%d\n\n", lista[i].letra, lista[i].numero);
    }
}
#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef enum {INSERIR = 1, LISTAR, PESQUISAR, ALTERAR, EXCLUIR, SAIR} Funcionalidade;

// Estrutura para armazenar informações de cartas
typedef struct {
    char nome[40];
    char letra;
    int numero;
    int super_trunfo;
    int influencia;
    int estrategia;
    int popularidade;
    int legado;
} Cartas;


void shuffleCartas(Cartas*array, int n);

void distribuiCartas(Cartas *array, int n, Cartas *jogador, Cartas *bot);

void comparaCartas(Cartas **jogador, int *tam_jogador, Cartas **bot, int *tam_bot);



#endif // GAME_H

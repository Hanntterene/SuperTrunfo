#ifndef CARTAS_H
#define CARTAS_H

#include <stdbool.h>

// Enumeração para funcionalidades
typedef enum {INSERIR = 1, LISTAR, PESQUISAR, ALTERAR, EXCLUIR, SAIR} Funcionalidade;

// Estrutura para armazenar informações de cartas
typedef struct {
    char nome[40];
    char letra;
    char numero;
    bool super_trunfo;
    int a1;
    int a2;
    int a3;
    int a4;
    bool carta_jogador;
} Cartas;

// Estrutura para cartas específicas do jogador
typedef struct {
    char nome[40];
    char letra;
    char numero;
    bool super_trunfo;
    int a1;
    int a2;
    int a3;
    int a4;
} Cartas_jogador;

// Funções para manipulação de cartas
void exibeCartas(Cartas *cartas, int i);

#endif // CARTAS_H

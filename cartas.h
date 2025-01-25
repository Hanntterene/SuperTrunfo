#ifndef CARTAS_H
#define CARTAS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Enumeração para funcionalidades
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

typedef struct {
    char nome[40];
    char letra;
    char numero;
    bool super_trunfo;
    int a1;
    int estrategia;
    int popularidade;
    int legado;
} Deck;

// Função para exibir informações de uma carta
void exibeCartas(Cartas *lista, int i);

// Função para preencher uma string (usada para entrada de dados)
void preencheString(char pesquisa_nome[]);

// Função para exibir atributos específicos de uma carta
void exibeAtributos(char pesquisa_nome[], Cartas lista[], int i);

// Função para pesquisar cartas por atributo ou ID (número e letra)
void pesquisaAtributoID(char pesquisa_nome[], int **selecao, int **tamanho, Cartas lista[]);

// Função para pesquisar cartas pelo nome
void pesquisaNome(char pesquisa_nome[], int *tamanho, Cartas lista[], bool* existencia, char* nova_pesquisa);

// Submenu para pesquisar cartas por atributo ou ID
void submenuAtributoID(char* nova_pesquisa, int* selecao, int* tamanho, Cartas lista[], char pesquisa_nome[]);

// Função para ler um número dentro de um intervalo definido
int lerNumero(const char *mensagem, int min, int max);

// Função para ler uma letra dentro de um conjunto de opções
char lerLetra(const char *mensagem, const char opcoes[]);

// Função para confirmar a escolha do usuário
int confirmarEscolha(const char *mensagem);

// Função para alterar atributos de uma carta
void alterarCarta(int *tamanho, Cartas lista[]);

// Função para excluir uma carta do deck
void excluirCarta(int *tamanho, Cartas lista[], Deck jogador[], Deck bot[]);

// Função adicionar cartas
void inserirCarta(int *tamanho, Cartas **lista, Deck **jogador, Deck **bot);


#endif // CARTAS_H

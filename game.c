#include <stdio.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "cartas.h"

void shuffleCartas(Cartas*array, int n) {
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Cartas temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Function to distribute Cartas into Cartas_jogador and Cartas_bot
void distribuiCartas(Cartas *array, int n, Cartas *jogador, Cartas *bot) {
    shuffleCartas(array, n);
    int metade = n / 2;
    if (n % 2 != 0) {
        printf("Número de cartas ímpar, impossível distribuir igualmente.\n");
        return;
    }
    for (int i = 0; i < metade; i++) {
        strcpy(jogador[i].nome, array[i].nome);
        jogador[i].letra = array[i].letra;
        jogador[i].numero = array[i].numero;
        jogador[i].super_trunfo = array[i].super_trunfo;
        jogador[i].influencia = array[i].influencia;
        jogador[i].estrategia = array[i].estrategia;
        jogador[i].popularidade = array[i].popularidade;
        jogador[i].legado = array[i].legado;
    }
    for (int i = metade; i < n; i++) {
        strcpy(bot[i - metade].nome, array[i].nome);
        bot[i - metade].letra = array[i].letra;
        bot[i - metade].numero = array[i].numero;
        bot[i - metade].super_trunfo = array[i].super_trunfo;
        bot[i - metade].influencia = array[i].influencia;
        bot[i - metade].estrategia = array[i].estrategia;
        bot[i - metade].popularidade = array[i].popularidade;
        bot[i - metade].legado = array[i].legado;
    }
}

// Compara os atributos e diz quem venceu a rodada
void comparaCartas(Cartas **jogador, int *tam_jogador, Cartas **bot, int *tam_bot) {
    int atributo;
    int jogador_valor, bot_valor;
    
    while (*tam_jogador > 0 && *tam_bot > 0) {
        for (int i = 0; i < *tam_jogador; i += 1) {
            exibeCartas(*jogador, i);
        }
        for (int i = 0; i < *tam_bot; i += 1) {
            exibeCartas(*bot, i);
        }
        // Exibe a carta atual do jogador
        printf((*jogador)[0].super_trunfo == 1 ? "SUPER TRUNFO\n" : "");
        printf("\nCarta Atual do Jogador:\n");
        printf("  Nome: %s\n", (*jogador)[0].nome);
        printf("  Atributos:\n");
        printf("    - Influência: %d\n", (*jogador)[0].influencia);
        printf("    - Estratégia: %d\n", (*jogador)[0].estrategia);
        printf("    - Popularidade: %d\n", (*jogador)[0].popularidade);
        printf("    - Legado: %d\n", (*jogador)[0].legado);
        printf("    - Código: %c-%d\n\n", (*jogador)[0].letra, (*jogador)[0].numero);

        // Solicita ao jogador que escolha um atributo para comparar
        printf("Escolha um atributo para comparar:\n");
        printf("1. Influencia\n");
        printf("2. Estrategia\n");
        printf("3. Popularidade\n");
        printf("4. Legado\n-> ");
        scanf("%d", &atributo);

        // Atribui os valores dos atributos escolhidos
        switch (atributo) {
            case 1:
                jogador_valor = (*jogador)[0].influencia;
                bot_valor = (*bot)[0].influencia;
                break;
            case 2:
                jogador_valor = (*jogador)[0].estrategia;
                bot_valor = (*bot)[0].estrategia;
                break;
            case 3:
                jogador_valor = (*jogador)[0].popularidade;
                bot_valor = (*bot)[0].popularidade;
                break;
            case 4:
                jogador_valor = (*jogador)[0].legado;
                bot_valor = (*bot)[0].legado;
                break;
            default:
                printf("Atributo inválido! Tente novamente.\n");
                continue;
        }

        // Comparação com Super Trunfo
        if ((*jogador)[0].super_trunfo == 1 && ((*bot)[0].letra == 'A' || (*bot)[0].letra == 'a')) {
            printf("\nBot venceu a rodada! Id A ganha do super trunfo\n");
        } else if ((*bot)[0].super_trunfo == 1 && ((*jogador)[0].letra == 'A' || (*jogador)[0].letra == 'a')) {
            printf("\nJogador venceu a rodada! Id A ganha do super trunfo\n");
        } else if ((*jogador)[0].super_trunfo == 1) {
            printf("\nJogador venceu a rodada! SUPER TRUNFO\n");
        } else if ((*bot)[0].super_trunfo == 1) {
            printf("\nBot venceu a rodada! SUPER TRUNFO\n");
        } else {
            // Compara os valores dos atributos escolhidos
            if (jogador_valor > bot_valor) {
                printf("\nJogador venceu a rodada!\n");
                printf((*bot)[0].super_trunfo == 1 ? "SUPER TRUNFO\n" : "");
                printf("Carta do Bot: \n");
                printf("  Nome: %s\n", (*bot)[0].nome);
                printf("  Atributos:\n");
                printf("    - Influência: %d\n", (*bot)[0].influencia);
                printf("    - Estratégia: %d\n", (*bot)[0].estrategia);
                printf("    - Popularidade: %d\n", (*bot)[0].popularidade);
                printf("    - Legado: %d\n", (*bot)[0].legado);
                printf("    - Código: %c-%d\n\n", (*bot)[0].letra, (*bot)[0].numero);
                printf("====================================================\n\n");
            } else if (jogador_valor < bot_valor) {
                printf("\nBot venceu a rodada!\n");
                printf((*bot)[0].super_trunfo == 1 ? "SUPER TRUNFO\n" : "");
                printf("Carta do Bot: \n");
                printf("  Nome: %s\n", (*bot)[0].nome);
                printf("  Atributos:\n");
                printf("    - Influência: %d\n", (*bot)[0].influencia);
                printf("    - Estratégia: %d\n", (*bot)[0].estrategia);
                printf("    - Popularidade: %d\n", (*bot)[0].popularidade);
                printf("    - Legado: %d\n", (*bot)[0].legado);
                printf("    - Código: %c-%d\n\n", (*bot)[0].letra, (*bot)[0].numero);
                printf("====================================================\n\n");
            } else {
                printf("\nEmpate\n");
                printf("Carta do Bot: \n");
                printf("  Nome: %s\n", (*bot)[0].nome);
                printf("  Atributos:\n");
                printf("    - Influência: %d\n", (*bot)[0].influencia);
                printf("    - Estratégia: %d\n", (*bot)[0].estrategia);
                printf("    - Popularidade: %d\n", (*bot)[0].popularidade);
                printf("    - Legado: %d\n", (*bot)[0].legado);
                printf("    - Código: %c-%d\n\n", (*bot)[0].letra, (*bot)[0].numero);
                printf("====================================================\n\n");
            }
        }

        // Move cartas para os respectivos decks
        Cartas carta_jogador = (*jogador)[0];
        Cartas carta_bot = (*bot)[0];

        // Atualiza o deck do jogador
        memmove(*jogador, *jogador + 1, (*tam_jogador - 1) * sizeof(Cartas));
        (*tam_jogador)--;
        *jogador = realloc(*jogador, (*tam_jogador) * sizeof(Cartas));

        // Atualiza o deck do bot
        memmove(*bot, *bot + 1, (*tam_bot - 1) * sizeof(Cartas));
        (*tam_bot)--;
        *bot = realloc(*bot, (*tam_bot) * sizeof(Cartas));

        // Adiciona a carta no baralho vencedor
        if (jogador_valor > bot_valor || (*jogador)[0].super_trunfo == 1) {
            *jogador = realloc(*jogador, (*tam_jogador + 2) * sizeof(Cartas));
            (*jogador)[*tam_jogador] = carta_jogador;
            (*jogador)[*tam_jogador + 1] = carta_bot;
            *tam_jogador += 2;
        } else if (jogador_valor < bot_valor || (*bot)[0].super_trunfo == 1) {
            *bot = realloc(*bot, (*tam_bot + 2) * sizeof(Cartas));
            (*bot)[*tam_bot] = carta_bot;
            (*bot)[*tam_bot + 1] = carta_jogador;
            *tam_bot += 2;
        } else {
            // Em caso de empate, cada carta retorna ao seu próprio baralho
            *jogador = realloc(*jogador, (*tam_jogador + 1) * sizeof(Cartas));
            (*jogador)[*tam_jogador] = carta_jogador;
            (*tam_jogador)++;

            *bot = realloc(*bot, (*tam_bot + 1) * sizeof(Cartas));
            (*bot)[*tam_bot] = carta_bot;
            (*tam_bot)++;
        }
    }

    // Verifica quem venceu o jogo
    if (*tam_jogador == 0) {
        printf("Bot venceu o jogo!\n\n\n");
    } else {
        printf("Jogador venceu o jogo!\n\n\n");
    }
}   

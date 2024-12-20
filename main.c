#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "cartas.h"

/**
 * @brief 
 * 
 * @file main.c
 * @author Nicolas, Lohanna, Fábio
 * @date 2024-12-04
*/


int main() {
    int tamanho = 32;
    char filename[] = "cartas.csv";
    int contador = 0;
    int coluna;
    int tamanhoBinario;
    char linhas[256];
    FILE *binario = fopen("save.bin", "rb");
    FILE *file = fopen(filename, "r");

    Cartas *lista = (Cartas*) malloc(sizeof(Cartas) * tamanho);
    if (lista == NULL) {
        printf("Não foi possível alocar dinâmicamente.\n");
        exit(1);
    }
    // Caso seja a primeira vez acessada, leitura pelo .csv
    if (binario == NULL) {
        if (file == NULL) {
            fprintf(stderr, "Não foi possivel abrir o arquivo %s: %s\n", filename, strerror(errno));
            exit(1);
        }

        printf("Seja bem-vindo!\n");

        while (fgets(linhas, sizeof(linhas), file)) {
            contador++; 

            // Pula a linha do cabeçalho
            if (contador == 1) {
                continue;
            }

            // Preenche a estrutura
            int index = contador - 2; // Índice no array 
            if (index >= tamanho) {
                break; // Garante que não ultrapasse o limite de n cartas, no caso 10
            }

            // Casos tokens
            char *token = strtok(linhas, ",");
            coluna = 0;
            while (token) {
                coluna++;
                switch (coluna) {
                    case 2: // Nome
                        strncpy(lista[index].nome, token, sizeof(lista[index].nome) - 1);
                        lista[index].nome[sizeof(lista[index].nome) - 1] = '\0';
                        break;
                    case 3:
                        lista[index].influencia = atoi(token); // Salvo como int (converte string -> int) 
                        break;
                    case 4: 
                        lista[index].estrategia = atoi(token);
                        break;
                    case 5: 
                        lista[index].popularidade = atoi(token);
                        break;
                    case 6: 
                        lista[index].super_trunfo = atoi(token);
                        break;
                    case 7:
                        lista[index].letra = token[0]; // Corrige para armazenar apenas o primeiro caractere
                        break;
                    case 8: 
                        lista[index].numero = atoi(token);
                        break;
                    case 9: 
                        lista[index].legado = atoi(token);
                        break;
                }
                token = strtok(NULL, ",");
            }
        }
    }
    else {
        printf("Bem vindo de volta!\n");
    
        fseek(binario, 0, SEEK_END);
        tamanhoBinario = ftell(binario);
        tamanho = tamanhoBinario / sizeof(Cartas);
        lista = realloc(lista, sizeof(Cartas) * tamanho);
        fseek(binario, 0, 0);
        fread(lista, sizeof(Cartas), tamanho, binario);
    }
    
    fclose(binario);
    fclose(file);

    // variáveis correspondentes as pesquisas de usuário
    int escolha;
    int tipo_pesquisa;
    int modo;
    int selecao;
    char pesquisa_nome[40];
    // int pesquisa_valor_atributo;
    bool existencia = 0;
    char nova_pesquisa = 'n'; // condição inicial negada


    // MENU INCIAL (Edição, início do jogo ou finalização do programa)
    do {
        printf("1 - EDITOR DE DECK\n");
        printf("2 - START\n");
        printf("3 - SAIR\n");

        printf("Selecione uma opção: ");
        scanf("%d", &modo);

        // MENU DE EDIÇÃO
        if (modo == 1) {
            do {
                printf("\n1 - INSERIR CARTA AO DECK\n");
                printf("2 - LISTA AS CARTAS DISPONÍVEIS\n");
                printf("3 - PESQUISAR CARTA NA LISTA DISPONÍVEL\n");
                printf("4 - ALTERAR CARTA NO DECK\n");
                printf("5 - EXCLUÍR CARTA DO DECK\n");
                printf("6 - SAIR DO EDITOR\n");

                printf("Escolha a opção desejada: ");
                scanf("%d", &escolha);
                switch (escolha) {
                    case INSERIR:
                        inserirCarta(&tamanho, &lista);
                    break;
                    case LISTAR:
                        for (int i = 0; i < tamanho; i += 1) {
                            exibeCartas(lista, i);
                        }

                    break;
                    case PESQUISAR:
                        do {

                            if (nova_pesquisa == 'n') { // Modo padrão pois nenhuma pesquisa foi realizada ou não foi solicitada
                                printf("\n1 - Nome da carta\n");
                                printf("2 - Atributo ou ID\n");
                                printf("3 - Voltar\n");

                                printf("Escolha o tipo de pesquisa: ");
                                scanf("%d", &tipo_pesquisa);
                            }
                            else { // modo de pesquisa rápida
                                tipo_pesquisa = 1;
                            }
                            
                            // Pesquisa por nome
                            if (tipo_pesquisa == 1) {
                                pesquisaNome(pesquisa_nome, &tamanho, lista, &existencia, &nova_pesquisa);
                            }
                            
                            // Pesquisa por atributo ou ID
                            else if (tipo_pesquisa == 2) {
                                submenuAtributoID(&nova_pesquisa, &selecao, &tamanho, lista, pesquisa_nome);
                            }

                            else if (tipo_pesquisa > 3)
                                printf("Opção não disponível...\n");
                        } while (tipo_pesquisa != 3);
                    break;

                    case ALTERAR:
                        alterarCarta(&tamanho, lista);
                    break;
                    case EXCLUIR:
                        excluirCarta(&tamanho, lista);
                    break;
                    case SAIR:
                
                    break;
                    default:
                        printf("Selecione uma opção válida.\n");
                    break;
                }
            } while (escolha != 6);
        } 

        else if (modo == 2) {
            printf("Desculpe o modo de batalha ainda não está disponível.\n");
        }

        else if (modo > 3 || modo < 1) {
            printf("Desulpe, essa opção não está disponível...\n");
        }
    } while (modo != 3);

    printf("Obrigado por jogar! Seus dados ficarão salvos para nosso próximo encontro!\n");

    free(lista);

    
    FILE* save = fopen("save.bin", "wb");
    fwrite(lista, sizeof(Cartas), tamanho, save);

    fclose(save);


    return 0;
}

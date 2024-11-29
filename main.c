#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cartas.h"

int main() {
    int tamanho = 10;
    char filename[] = "C:\\AA - Faculdade\\C\\Trabalho segundo periodo\\cartas.csv";
    int contador = 0;
    int coluna;
    char linhas[256];
    Cartas lista[tamanho];
    //Cartas deck[tamanho / 2];
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Não foi possivel abrir o arquivo %s: %s\n", filename, strerror(errno));
        exit(1);
    }

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
                    lista[index].a1 = atoi(token); // Salvo como int (converte string -> int) 
                    break;
                case 4: 
                    lista[index].a2 = atoi(token);
                    break;
                case 5: 
                    lista[index].a3 = atoi(token);
                    break;
                case 6: 
                    lista[index].a4 = atoi(token);
                    break;
                case 7:
                    lista[index].letra = token[0]; // Corrige para armazenar apenas o primeiro caractere
                    break;
                case 8: 
                    lista[index].numero = atoi(token);
                    break;
                case 9: 
                    lista[index].super_trunfo = atoi(token);
                    break;
            }
            token = strtok(NULL, ",");
        }
    }

    fclose(file);

    int escolha;
    int tipo_pesquisa;
    int modo;
    char pesquisa_nome[40];


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
                        
                    break;
                    case LISTAR:
                        for (int i = 0; i < tamanho; i += 1) {
                            exibeCartas(lista, i);
                        }

                    break;
                    case PESQUISAR:
                        do {
                            printf("\n1 - Nome da carta\n");
                            printf("2 - Atributo\n");
                            printf("3 - Voltar\n");

                            printf("Escolha o tipo de pesquisa: ");
                            scanf("%d", &tipo_pesquisa);
                            if (tipo_pesquisa == 1) {
                                setbuf(stdin, NULL);
                                printf("Digite o nome da carta a ser pesquisada: ");
                                fgets(pesquisa_nome, 40, stdin);
                                pesquisa_nome[strcspn(pesquisa_nome, "\n")] = '\0';
                                for (int i = 0; i < tamanho; i += 1) {
                                    if (strcmp(pesquisa_nome, lista[i].nome) == 0) {
                                        exibeCartas(lista, i);
                                    }
                                }
                            }
                        } while (tipo_pesquisa != 3);
                    break;

                    case ALTERAR:

                    break;
                    case EXCLUIR:

                    break;
                    case SAIR:

                    break;
                    default:
                        printf("Selecione uma opção válida.\n");
                    break;
                }
            } while (escolha != 6);
        }   
    } while (modo != 3);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/**
 * @brief 
 * 
 * @file main.c
 * @author Nicolas, Lohanna, Fábio
 * @date 2024-12-04
*/


/**
 * @brief enum functionalities
 * 
 */
typedef enum {INSERIR = 1, LISTAR, PESQUISAR, ALTERAR, EXCLUIR, SAIR} Funcionalidade;

/**
 * @brief struct Cartas
 * 
 */
typedef struct {
    char nome[40];
    char letra;
    char numero;
    bool super_trunfo;
    int influencia;
    int estrategia;
    int popularidade;
    int legado;
    bool carta_jogador;
} Cartas;

/* Estrutura para cartas específicas do jogador
typedef struct {
    char nome[40];
    char letra;
    char numero;
    bool super_trunfo;
    int influencia;
    int estrategia;
    int popularidade;
    int legado;
} Cartas_jogador;
*/

void exibeCartas (Cartas *lista, int i) {
    if (lista[i].super_trunfo == 1) {
        printf("SUPER TRUNFO\n");
        printf("Carta %d:\n", i + 1);
        printf("  Nome: %s\n", lista[i].nome);
        printf("  Atributos:\n");
        printf("    - Influência: %d\n", lista[i].influencia);
        printf("    - Estratégia: %d\n", lista[i].estrategia);
        printf("    - Popularidade: %d\n", lista[i].popularidade);
        printf("    - Legado: %d\n", lista[i].legado);
        printf("  Atributos: %d, %d, %d, %d\n", lista[i].influencia, lista[i].estrategia, lista[i].popularidade, lista[i].legado);
        printf("  Código: %c-%d\n\n", lista[i].letra, lista[i].numero);
    }
    else {
        printf("Carta %d:\n", i + 1);
        printf("  Nome: %s\n", lista[i].nome);
        printf("  Atributos:\n");
        printf("    - Influência: %d\n", lista[i].influencia);
        printf("    - Estratégia: %d\n", lista[i].estrategia);
        printf("    - Popularidade: %d\n", lista[i].popularidade);
        printf("    - Legado: %d\n", lista[i].legado);
        printf("  Atributos: %d, %d, %d, %d\n", lista[i].influencia, lista[i].estrategia, lista[i].popularidade, lista[i].legado);
        printf("  Código: %c-%d\n\n", lista[i].letra, lista[i].numero);
    }
}

void preencheString (char pesquisa_nome[]) {
    setbuf(stdin, NULL);
    printf("Digite o nome da carta a ser pesquisada: ");
    fgets(pesquisa_nome, 40, stdin);
    pesquisa_nome[strcspn(pesquisa_nome, "\n")] = '\0';
}

void exibeAtributos (char pesquisa_nome[], Cartas lista[], int i) {
    if(strcmp(pesquisa_nome, "influencia") == 0) {
        printf("-> %s", lista[i].nome);
        printf(" Influência: %d\n", lista[i].influencia);
        }
        else if(strcmp(pesquisa_nome, "estrategia") == 0) {
        printf("-> %s", lista[i].nome);
        printf(" Estratégia: %d\n", lista[i].estrategia);
        }
        else if(strcmp(pesquisa_nome, "popularidade") == 0) {
        printf("-> %s", lista[i].nome);
        printf(" Popularidade: %d\n", lista[i].popularidade);
        }
        else if(strcmp(pesquisa_nome, "legado") == 0) {
        printf("-> %s", lista[i].nome);
        printf(" Legado: %d\n", lista[i].legado);
        }
        else {
        printf("Não foi encontrado nenhum atributo referente a pesquisa\n");
        }
}

void pesquisaAtributoID (char pesquisa_nome[], int selecao) {
    int ID_numero;
    char ID_letra;

    setbuf(stdin, NULL);
    if (selecao == 1) {
        printf("Digite o atributo a ser pesquisado: ");
        fgets(pesquisa_nome, 20, stdin);
    }
    else {
        do {
            printf("Digite o número do ID (1-8): ");
            scanf("%d", &ID_numero);
        } while (ID_numero > 8 || ID_numero < 1);

        setbuf(stdin, NULL);
        do {
            printf("Digite uma letra do ID (A - D): ");
            scanf("%c", &ID_letra);
            ID_letra = tolower(ID_letra);
        } while (ID_letra != 'a' || ID_letra != 'b' || ID_letra != 'c' || ID_letra != 'd');

    }
    pesquisa_nome[strcspn(pesquisa_nome, "\n")] = '\0';
}

int main() {
    int tamanho = 32;
    char filename[] = "C:\\Users\\rstra\\OneDrive\\Documentos\\GitHub\\SuperTrunfo\\cartas.csv";
    int contador = 0;
    int coluna;
    char linhas[256];
    Cartas *lista = (Cartas*) malloc(sizeof(Cartas) * tamanho);
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
                                printf("2 - Atributo\n");
                                printf("3 - Voltar\n");

                                printf("Escolha o tipo de pesquisa: ");
                                scanf("%d", &tipo_pesquisa);
                            }
                            else { // modo de pesquisa rápida
                                tipo_pesquisa = 1;
                            }

                            if (tipo_pesquisa == 1) {
                                preencheString(pesquisa_nome);
                                for (int i = 0; i < tamanho; i += 1) {
                                    if (strcmp(pesquisa_nome, lista[i].nome) == 0) {
                                        exibeCartas(lista, i);
                                        existencia = 1;
                                    }
                                }
                                if (existencia == 0) {
                                    printf("Não foi possível encontrar nenhuma carta com o nome acima...\n");
                                }
                                do {
                                    printf("Deseja pesquisar novamente? (s/n)\n");
                                    printf("-> ");
                                    setbuf(stdin, NULL);
                                    scanf("%c", &nova_pesquisa);
                                    nova_pesquisa = tolower(nova_pesquisa);
                                } while (nova_pesquisa != 's' || nova_pesquisa != 'n');
                                existencia = 0;
                            }

                            if (tipo_pesquisa == 2) {
                                do {
                                    if (nova_pesquisa == 'n') { // Modo padrão
                                        printf("   1 - Atributo\n");
                                        printf("   2 - ID (Letra e Número)\n");
                                        printf("   3 - Voltar\n");
                                    
                                        printf("Escolha o tipo de pesquisa: ");
                                        scanf("%d", &selecao);
                                    }

                                    if (selecao == 1) {
                                        pesquisaAtributoID(pesquisa_nome, selecao);
                                        
                                        for (int i = 0; i < tamanho; i += 1) {
                                            exibeAtributos(pesquisa_nome, lista, i);
                                        }
                                        do {
                                            printf("Deseja pesquisar outro nome? (s/n): ");
                                            setbuf(stdin, NULL);
                                            printf("-> ");
                                            scanf("%c", &nova_pesquisa);
                                            nova_pesquisa = tolower(nova_pesquisa);
                                        } while (nova_pesquisa != 'n' || nova_pesquisa != 's');
                                    }

                                    else if (selecao == 2){
                                        pesquisaAtributoID(pesquisa_nome, selecao);
                                    }

                                    else if (selecao > 3)
                                        printf("Desculpe esse modo não está disponível, selecione um modo válido.\n");
                                } while (selecao != 3);
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

        else if (modo == 2) {
            printf("Desculpe o modo de batalha ainda não está disponível.\n");
        }

        else if (modo > 3 || modo < 1) {
            printf("Desulpe, essa opção não está disponível...\n");
        }
    } while (modo != 3);

    printf("Obrigado por jogar! Seus dados ficarão salvos para nosso próximo encontro!\n");
    return 0;
}

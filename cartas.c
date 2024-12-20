#include <stdio.h>
#include <string.h>
#include "cartas.h"


/**
 * @brief 
 * 
 * @param lista 
 * @param i 
 */
void exibeCartas(Cartas *lista, int i) {
    printf(lista[i].super_trunfo == 1 ? "SUPER TRUNFO\n" : "");
    printf("Carta %d:\n", i + 1);
    printf("  Nome: %s\n", lista[i].nome);
    printf("  Atributos:\n");
    printf("    - Influência: %d\n", lista[i].influencia);
    printf("    - Estratégia: %d\n", lista[i].estrategia);
    printf("    - Popularidade: %d\n", lista[i].popularidade);
    printf("    - Legado: %d\n", lista[i].legado);
    printf("    - Código: %c-%d\n\n", lista[i].letra, lista[i].numero);
}

/**
 * @brief 
 * 
 * @param pesquisa_nome 
 */
void preencheString (char pesquisa_nome[]) {
    setbuf(stdin, NULL);
    fgets(pesquisa_nome, 40, stdin);
    pesquisa_nome[strcspn(pesquisa_nome, "\n")] = '\0';
}

/**
 * @brief 
 * 
 * @param pesquisa_nome 
 * @param lista 
 * @param i 
 */
void exibeAtributos (char pesquisa_nome[], Cartas lista[], int i) {
    if(strcmpi(pesquisa_nome, "influencia") == 0) {
        printf("-> %s", lista[i].nome);
        printf(" Influência: %d\n", lista[i].influencia);
        }
        else if(strcmpi(pesquisa_nome, "estrategia") == 0) {
        printf("-> %s", lista[i].nome);
        printf(" Estratégia: %d\n", lista[i].estrategia);
        }
        else if(strcmpi(pesquisa_nome, "popularidade") == 0) {
        printf("-> %s", lista[i].nome);
        printf(" Popularidade: %d\n", lista[i].popularidade);
        }
        else if(strcmpi(pesquisa_nome, "legado") == 0) {
        printf("-> %s", lista[i].nome);
        printf(" Legado: %d\n", lista[i].legado);
        }
}

/**
 * @brief 
 * 
 * @param pesquisa_nome 
 * @param selecao 
 * @param tamanho 
 * @param lista 
 */
void pesquisaAtributoID (char pesquisa_nome[], int **selecao, int ** tamanho, Cartas lista[]) {
    int ID_numero;
    char ID_letra;

    setbuf(stdin, NULL);
    if (**selecao == 1) {
        printf("Digite o atributo a ser pesquisado: ");
        fgets(pesquisa_nome, 20, stdin);
        pesquisa_nome[strcspn(pesquisa_nome, "\n")] = '\0';
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
            ID_letra = toupper(ID_letra);
        } while (ID_letra != 'A' && ID_letra != 'B' && ID_letra != 'C' && ID_letra != 'D');

        for (int i = 0; i < **tamanho; i += 1) {
            if (lista[i].numero == ID_numero && lista[i].letra == ID_letra) {
                exibeCartas(lista, i);
            }
        }

    }
}

/**
 * @brief 
 * 
 * @param pesquisa_nome 
 * @param tamanho 
 * @param lista 
 * @param existencia 
 * @param nova_pesquisa 
 */
void pesquisaNome (char pesquisa_nome[], int *tamanho, Cartas lista[], bool* existencia, char* nova_pesquisa) {
    printf("Digite o nome da carta a ser pesquisada: ");
    preencheString(pesquisa_nome);
    for (int i = 0; i < *tamanho; i += 1) {
        if (strcmpi(pesquisa_nome, lista[i].nome) == 0) {
            exibeCartas(lista, i);
            *existencia = 1;
        }
    }
    if (*existencia == 0) {
        printf("Não foi possível encontrar nenhuma carta com o nome acima...\n");
    }
    do {
        printf("Deseja pesquisar novamente? (s/n)\n");
        printf("-> ");
        setbuf(stdin, NULL);
        scanf("%c", nova_pesquisa);
        *nova_pesquisa = tolower(*nova_pesquisa);
    } while (*nova_pesquisa != 's' && *nova_pesquisa != 'n');
    *existencia = 0;
}

/**
 * @brief 
 * 
 * @param nova_pesquisa 
 * @param selecao 
 * @param tamanho 
 * @param lista 
 * @param pesquisa_nome 
 */
void submenuAtributoID (char* nova_pesquisa, int* selecao, int* tamanho, Cartas lista[], char pesquisa_nome[]) {
    do {
        if (*nova_pesquisa == 'n') { // Modo padrão
            printf("   1 - Atributo\n");
            printf("   2 - ID (Letra e Número)\n");
            printf("   3 - Voltar\n");
                                    
            printf("Escolha o tipo de pesquisa: ");
            scanf("%d", selecao);
        }

        if (*selecao == 1) {
            pesquisaAtributoID(pesquisa_nome, &selecao, &tamanho, lista);
                                        
            for (int i = 0; i < *tamanho; i += 1) {
                exibeAtributos(pesquisa_nome, lista, i);
            }
            do {
                printf("Deseja pesquisar outro nome? (s/n): ");
                setbuf(stdin, NULL);
                printf("-> ");
                scanf("%c", nova_pesquisa);
                *nova_pesquisa = tolower(*nova_pesquisa);
            } while (*nova_pesquisa != 'n' && *nova_pesquisa != 's');
        }

        else if (*selecao == 2){
            pesquisaAtributoID(pesquisa_nome, &selecao, &tamanho, lista);
            do {
                printf("Deseja pesquisar outro nome? (s/n): ");
                setbuf(stdin, NULL);
                printf("-> ");
                scanf("%c", nova_pesquisa);
                *nova_pesquisa = tolower(*nova_pesquisa);
            } while (*nova_pesquisa != 'n' && *nova_pesquisa != 's');
        }

        else if (*selecao > 3)
            printf("Desculpe esse modo não está disponível, selecione um modo válido.\n");
    } while (*selecao != 3);
}

/**
 * @brief 
 * 
 * @param mensagem 
 * @param min 
 * @param max 
 * @return int 
 */
int lerNumero(const char *mensagem, int min, int max) {
    int numero;
    do {
        printf("%s (%d - %d): ", mensagem, min, max);
        scanf("%d", &numero);
        if (numero < min || numero > max) {
            printf("Valor inválido! Tente novamente.\n");
        }
    } while (numero < min || numero > max);
    return numero;
}

/**
 * @brief 
 * 
 * @param mensagem 
 * @param opcoes 
 * @return char 
 */
char lerLetra(const char *mensagem, const char opcoes[]) {
    char letra;
    int valido;
    do {
        printf("%s (%s): ", mensagem, opcoes);
        setbuf(stdin, NULL);
        scanf("%c", &letra);
        letra = toupper(letra);
        valido = (strchr(opcoes, letra) != NULL);
        if (!valido) {
            printf("Letra inválida! Tente novamente.\n");
        }
    } while (!valido);
    return letra;
}

/**
 * @brief Confirma a escolha do usuário de pesquisar estar correta ou pesquisar novamente a carta 
 * 
 * @param mensagem 
 * @return int 
 */
int confirmarEscolha(const char *mensagem) {
    int escolha;
    do {
        printf("%s\n1 - Sim\n2 - Não\nEscolha: ", mensagem);
        scanf("%d", &escolha);
        if (escolha != 1 && escolha != 2) {
            printf("Opção inválida! Tente novamente.\n");
        }
    } while (escolha != 1 && escolha != 2);
    return escolha;
}

/**
 * @brief Altera atributos selecionados pelo usuário
 * 
 * @param tamanho 
 * @param lista 
 */
void alterarCarta(int *tamanho, Cartas lista[]) {
    int ID_numero, atributo_mod;
    char ID_letra;
    int check = 0;

    ID_numero = lerNumero("Digite o número do ID", 1, 8);
    ID_letra = lerLetra("Digite a letra do ID", "ABCD");

    int encontrado = 0;  // Flag para verificar se encontramos a carta
    for (int i = 0; i < *tamanho; i++) {
        // Verifica se a carta com o número e letra corresponde
        if (lista[i].numero == ID_numero && lista[i].letra == ID_letra) {
            exibeCartas(lista, i);
            encontrado = 1; // Marca que encontramos uma carta com esse ID

            if (confirmarEscolha("É essa carta que deseja alterar?") == 1) {
                // Verifica se a carta não é a "Muriel Godoi"
                if (strcmp(lista[i].nome, "Muriel Godoi") != 0) {
                    do {
                        printf("Selecione o atributo que deseja alterar:\n");
                        printf("1 - Legado\n2 - Popularidade\n3 - Estratégia\n4 - Influência\nEscolha: ");
                        scanf("%d", &atributo_mod);

                        switch (atributo_mod) {
                            case 1:
                                printf("Alterando o atributo Legado...\n");
                                do {
                                    printf("Digite o novo valor de legado a ser alocado: ");
                                    scanf("%d", &lista[i].legado);
                                    if (lista[i].legado > 20 || lista[i].legado < 0) 
                                        printf("Valor inválido... valores permitidos 0 a 20\n");
                                } while (lista[i].legado > 20 || lista[i].legado < 0); 
                                break;

                            case 2:
                                printf("Alterando o atributo Popularidade...\n");
                                do {
                                    printf("Digite o novo valor de popularidade a ser alocado: ");
                                    scanf("%d", &lista[i].popularidade);
                                    if (lista[i].popularidade > 100 || lista[i].popularidade < 0) 
                                        printf("Valor inválido... valores permitidos 0 a 100\n");
                                } while (lista[i].popularidade > 100 || lista[i].popularidade < 0);
                                break;

                            case 3:
                                printf("Alterando o atributo Estratégia...\n");
                                do {
                                    printf("Digite o novo valor de estratégia a ser alocado: ");
                                    scanf("%d", &lista[i].estrategia);
                                    if (lista[i].estrategia > 100 || lista[i].estrategia < 0) 
                                        printf("Valor inválido... valores permitidos 0 a 100\n");
                                } while (lista[i].estrategia > 100 || lista[i].estrategia < 0); 
                                break;

                            case 4:
                                printf("Alterando o atributo Influência...\n");
                                do {
                                    printf("Digite o novo valor de influência a ser alocado: ");
                                    scanf("%d", &lista[i].influencia);
                                    if (lista[i].influencia > 5 || lista[i].influencia < 0) 
                                        printf("Valor inválido... valores permitidos 0 a 5\n");
                                } while (lista[i].influencia > 5 || lista[i].influencia < 0); 
                                break;

                            default:
                                printf("Opção inválida! Tente novamente.\n");
                                break;
                        }
                    } while (atributo_mod < 1 || atributo_mod > 4);
                } else {
                    printf("Essa carta não pode ser alterada ou excluída, o Criador é inalterável.\n");
                    return;  // Não permite alteração
                }
                break; // Sai do loop após a alteração
            } else {
                // Caso o usuário não queira alterar essa carta, ofereça a opção de selecionar outra carta
                if (confirmarEscolha("Deseja selecionar outra carta?") == 2) {
                    return;  // Sai da função sem fazer alteração
                }
                else {
                    // Verifica a existência de outras cartas com o mesmo ID, passando por todas
                    check = 0;
                    for (int j = i + 1; j < *tamanho; j += 1) {
                        if (lista[j].numero == ID_numero && lista[j].letra == ID_letra) {
                            check = 1;
                        }
                    }
                    if (check == 0) {
                        alterarCarta(tamanho, lista);
                    }
                }
                // Caso o usuário deseje continuar, o loop continuará para a próxima carta
            }
        }
    }

    // Caso nenhuma carta tenha sido encontrada, informe o usuário
    if (!encontrado) {
        printf("Carta com esse ID e letra não encontrada.\n");
    }
}

// Função para excluir uma carta do deck
void excluirCarta(int *tamanho, Cartas lista[]) {
    char pesquisa_nome[50];
    int existencia = 0, indice = -1;

    printf("Digite o nome da carta que deseja excluir: ");
    preencheString(pesquisa_nome);

    // Busca a carta pelo nome
    for (int i = 0; i < *tamanho; i++) {
        if (strcasecmp(lista[i].nome, pesquisa_nome) == 0) {
            existencia = 1;
            indice = i;
            break;
        }
    }

    if (existencia) {
        printf("Carta encontrada: %s\n", lista[indice].nome);
        printf("Tem certeza de que deseja excluí-la? (s/n): ");
        char confirmacao;
        scanf(" %c", &confirmacao);

        if (confirmacao == 's' || confirmacao == 'S') {
            // Realoca o vetor removendo a carta
            for (int i = indice; i < *tamanho - 1; i++) {
                lista[i] = lista[i + 1];
            }

            (*tamanho)--;
            lista = realloc (lista, sizeof(Cartas) * (*tamanho));

            if (lista == NULL && *tamanho > 0) {
                printf("Erro ao realocar memória após exclusão.\n");
                exit(1);
            }

            printf("Carta excluída com sucesso!\n");
        } else {
            printf("Exclusão cancelada.\n");
        }
    } else {
        printf("Carta com o nome '%s' não encontrada.\n", pesquisa_nome);
    }
}

// Função adicionar cartas
void inserirCarta(int *tamanho, Cartas **lista) {
    (*tamanho)++;
    *lista = realloc(*lista, sizeof(Cartas) * (*tamanho));
    if (*lista == NULL) {
        printf("Não foi possível alocar dinamicamente.\n");
        exit(1);
    }

    printf("Digite o nome da nova carta a ser incluída: ");
    preencheString((*lista)[*tamanho - 1].nome);

    do {
        printf("Digite o número do ID (1 a 8): ");
        scanf("%d", &(*lista)[*tamanho - 1].numero);
        if ((*lista)[*tamanho - 1].numero > 8 || (*lista)[*tamanho - 1].numero < 1)
            printf("Número do ID inválido... números permitidos de 1 a 8\n");
    } while ((*lista)[*tamanho - 1].numero > 8 || (*lista)[*tamanho - 1].numero < 1);

    do {
        printf("Digite a letra do ID (A a D): ");
        scanf(" %c", &(*lista)[*tamanho - 1].letra);  // Corrigido para consumir espaços e quebras de linha
        (*lista)[*tamanho - 1].letra = toupper((*lista)[*tamanho - 1].letra);
        if ((*lista)[*tamanho - 1].letra != 'A' && (*lista)[*tamanho - 1].letra != 'B' && (*lista)[*tamanho - 1].letra != 'C' && (*lista)[*tamanho - 1].letra != 'D') {
            printf("Letra do ID inválida... letras permitidas de A a D\n");
        }
    } while ((*lista)[*tamanho - 1].letra != 'A' && (*lista)[*tamanho - 1].letra != 'B' && (*lista)[*tamanho - 1].letra != 'C' && (*lista)[*tamanho - 1].letra != 'D');


    do {
        printf("Digite o valor da influência a ser alocado: ");
        scanf("%d", &(*lista)[*tamanho - 1].influencia);
        if ((*lista)[*tamanho - 1].influencia > 5 || (*lista)[*tamanho - 1].influencia < 0)
            printf("Valor inválido... valores permitidos 0 a 5\n");
    } while ((*lista)[*tamanho - 1].influencia > 5 || (*lista)[*tamanho - 1].influencia < 0);

    do {
        printf("Digite o valor da estratégia a ser alocado: ");
        scanf("%d", &(*lista)[*tamanho - 1].estrategia);
        if ((*lista)[*tamanho - 1].estrategia > 100 || (*lista)[*tamanho - 1].estrategia < 0)
            printf("Valor inválido... valores permitidos 0 a 100\n");
    } while ((*lista)[*tamanho - 1].estrategia > 100 || (*lista)[*tamanho - 1].estrategia < 0);

    do {
        printf("Digite o valor da popularidade a ser alocado: ");
        scanf("%d", &(*lista)[*tamanho - 1].popularidade);
        if ((*lista)[*tamanho - 1].popularidade > 100 || (*lista)[*tamanho - 1].popularidade < 0)
            printf("Valor inválido... valores permitidos 0 a 100\n");
    } while ((*lista)[*tamanho - 1].popularidade > 100 || (*lista)[*tamanho - 1].popularidade < 0);

    do {
        printf("Digite o valor do legado a ser alocado: ");
        scanf("%d", &(*lista)[*tamanho - 1].legado);
        if ((*lista)[*tamanho - 1].legado > 20 || (*lista)[*tamanho - 1].legado < 0)
            printf("Valor inválido... valores permitidos 0 a 20\n");
    } while ((*lista)[*tamanho - 1].legado > 20 || (*lista)[*tamanho - 1].legado < 0);

    do {
        printf("Digite 1 para que a carta seja super-trunfo, caso contrário digite 0: ");
        scanf("%d", &(*lista)[*tamanho - 1].super_trunfo);
    } while ((*lista)[*tamanho - 1].super_trunfo != 0 && (*lista)[*tamanho - 1].super_trunfo != 1);
}

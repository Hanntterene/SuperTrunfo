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
    int numero;
    int super_trunfo;
    int influencia;
    int estrategia;
    int popularidade;
    int legado;
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

void preencheString (char pesquisa_nome[]) {
    setbuf(stdin, NULL);
    fgets(pesquisa_nome, 40, stdin);
    pesquisa_nome[strcspn(pesquisa_nome, "\n")] = '\0';
}

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



int main() {
    int tamanho = 32;
    char filename[] = "C:\\Users\\rstra\\OneDrive\\Documentos\\GitHub\\SuperTrunfo\\cartas.csv";
    int contador = 0;
    int coluna;
    char linhas[256];
    bool mudancaCartas = false;

    Cartas *lista = (Cartas*) malloc(sizeof(Cartas) * tamanho);
    if (lista == NULL) {
        printf("Não foi possível alocar dinâmicamente.\n");
        exit(1);
    }
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

    // Fecha o arquivo cartas.csv
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
                        mudancaCartas = true;
                        tamanho++;
                        lista = realloc(lista, sizeof(Cartas) * tamanho);
                        
                        printf("Digite o nome da nova carta a ser incluída: ");
                        preencheString(lista[tamanho - 1].nome);
                        
                        do {
                            printf("Digite o número do ID (1 a 8): ");
                            scanf("%d", &lista[tamanho - 1].numero);
                            if (lista[tamanho - 1].numero > 8 || lista[tamanho - 1].numero < 0)
                                printf("Número do ID inválido... números permitidos de 1 a 8\n");
                        } while (lista[tamanho - 1].numero > 8 || lista[tamanho - 1].numero < 0);

                       do {
                            printf("Digite a letra do ID (A a D): ");
                            scanf(" %c", &lista[tamanho - 1].letra);  // Corrigido para consumir espaços e quebras de linha
                            if (lista[tamanho - 1].letra != 'a' && lista[tamanho - 1].letra != 'b' && lista[tamanho - 1].letra != 'c' && lista[tamanho - 1].letra != 'd') {
                                printf("Letra do ID inválida... letras permitidas de A a D\n");
                            }
                        } while (lista[tamanho - 1].letra != 'a' && lista[tamanho - 1].letra != 'b' && lista[tamanho - 1].letra != 'c' && lista[tamanho - 1].letra != 'd');
                        lista[tamanho - 1].letra = toupper(lista[tamanho - 1].letra);

                        do {
                            printf("Digite o valor da influência a ser alocado: ");
                            scanf("%d", &lista[tamanho - 1].influencia);
                            if (lista[tamanho - 1].influencia > 5 || lista[tamanho - 1].influencia < 0) 
                            printf("Valor inválido... valores permitidos 0 a 5\n");
                        } while (lista[tamanho - 1].influencia > 5 || lista[tamanho - 1].influencia < 0); 
                        
                        do {
                            printf("Digite o valor da estratégia a ser alocado: ");
                            scanf("%d", &lista[tamanho - 1].estrategia);
                            if (lista[tamanho - 1].estrategia > 100 || lista[tamanho - 1].estrategia < 0) 
                            printf("Valor inválido... valores permitidos 0 a 100\n");
                        } while (lista[tamanho - 1].estrategia > 100 || lista[tamanho - 1].estrategia < 0);

                        do {
                            printf("Digite o valor da popularidade a ser alocado: ");
                            scanf("%d", &lista[tamanho - 1].popularidade);
                            if (lista[tamanho - 1].popularidade > 100 || lista[tamanho - 1].popularidade < 0) 
                            printf("Valor inválido... valores permitidos 0 a 100\n");
                        } while (lista[tamanho - 1].popularidade > 100 || lista[tamanho - 1].popularidade < 0);

                        do {
                            printf("Digite o valor do legado a ser alocado: ");
                            scanf("%d", &lista[tamanho - 1].legado);
                            if (lista[tamanho - 1].legado > 20 || lista[tamanho - 1].legado < 0) 
                            printf("Valor inválido... valores permitidos 0 a 20\n");
                        } while (lista[tamanho - 1].legado > 20 || lista[tamanho - 1].legado < 0);

                        do {
                            printf("Digite 1 para que a carta seja super-trunfo, caso contrário digite 0: ");
                            scanf("%d", &lista[tamanho - 1].super_trunfo);
                        } while (lista[tamanho - 1].super_trunfo != 0 && lista[tamanho - 1].super_trunfo != 1);
                        
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
                        mudancaCartas = true;
                        alterarCarta(&tamanho, lista);
                    break;
                    case EXCLUIR: 
                        mudancaCartas = true;

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

    /* if (mudancaCartas == true) {
        FILE* save = fopen("save.csv", "w");
        fprintf(save, "ID,nome,influencia,estrategia,popularidade,super_trunfo,letra,numero,legado\n");

        for (int i = 0; i < tamanho; i += 1) {
            fprintf(save, "%d,%s,%d,%d,%d,%d,%c,%d,%d", i + 1, lista[i].nome, lista[i].influencia, lista[i].estrategia, lista[i].popularidade, lista[i].super_trunfo, lista[i].letra, lista[i].numero, lista[i].legado);
        }
    } */

    return 0;
}

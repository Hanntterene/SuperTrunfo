#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Estrutura que representa uma carta
typedef struct
{
    char nome[40];    // Nome da carta
    char letra;       // Letra identificadora da carta
    int numero;       // Número da carta
    int super_trunfo; // Indica se é um super trunfo
    int influencia;   // Atributo de influência
    int estrategia;   // Atributo de estratégia
    int popularidade; // Atributo de popularidade
    int legado;       // Atributo de legado
} Cartas;

// Função para limpar o buffer de entrada
void limpar_buffer()
{
    int ch;
    // Consome todos os caracteres até encontrar um '\n' ou EOF
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        // Apenas consome os caracteres
    }
}

// Função para embaralhar as cartas
void shuffleCartas(Cartas *array, int n)
{
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1); // Gera um índice aleatório
        Cartas temp = array[i];   // Troca as cartas
        array[i] = array[j];
        array[j] = temp;
    }
}

// Função para distribuir as cartas entre o jogador e o bot
void distribuiCartas(Cartas *array, int n, Cartas *jogador, Cartas *bot)
{
    shuffleCartas(array, n); // Embaralha as cartas
    int metade = n / 2;      // Calcula a metade do número de cartas

    // Verifica se o número de cartas é par
    if (n % 2 != 0)
    {
        printf("Número de cartas ímpar, impossível distribuir igualmente.\n");
        return;
    }

    // Distribui as cartas para o jogador
    for (int i = 0; i < metade; i++)
    {
        jogador[i] = array[i]; // Copia a carta para o jogador
    }
    // Distribui as cartas para o bot
    for (int i = metade; i < n; i++)
    {
        bot[i - metade] = array[i]; // Copia a carta para o bot
    }
}

// Enum para as telas do jogo
typedef enum
{
    MAIN_MENU,
    GAME,
    ACHIEVEMENTS,
    CONFIRM_EXIT,
    GAME_2,
    END_GAME,
    GAME_Maquina,
    GAME_Player
} Screen;

// enum para determinar quem joga no momento
typedef enum
{
    JOGADOR,
    MAQUINA
} Rodada;

int main()
{
    // Inicialização da janela
    const int screenWidth = 1200;  // Largura da tela
    const int screenHeight = 1000; // Altura da tela

    InitWindow(screenWidth, screenHeight, "SuperTrunfo"); // Cria a janela do jogo
    SetTargetFPS(60);                                     // Define a taxa de quadros

    // Carregamento de imagens
    Image cartaPla = LoadImage("resources/cat.png");
    ImageResize(&cartaPla, 200, 250); // Redimensiona a imagem da carta do jogador

    Image cartaMaq = LoadImage("resources/cat.png");
    ImageResize(&cartaMaq, 200, 250); // Redimensiona a imagem da carta da máquina

    Texture2D cartaPlayer = LoadTextureFromImage(cartaPla); // Cria textura da carta do jogador
    UnloadImage(cartaPla);                                  // Libera a imagem original

    Texture2D cartaMaquina = LoadTextureFromImage(cartaMaq); // Cria textura da carta da máquina
    UnloadImage(cartaMaq);                                   // Libera a imagem original

    // Inicialização das variáveis de controle de tela
    Screen currentScreen = MAIN_MENU;    // Tela atual
    Screen previousScreen = MAIN_MENU;   // Tela anterior
    GuiSetStyle(DEFAULT, TEXT_SIZE, 50); // Define o estilo dos botões

    // Dimensões das cartas
    int cardWidth = cartaPlayer.width;
    int cardHeight = cartaPlayer.height;

    // Posições das cartas
    int cardY = screenHeight / 2 - cardHeight / 2 - 50; // Posição Y centralizada
    int playerCardX = screenWidth / 2 - cardWidth - 20; // Posição X da carta do jogador
    int machineCardX = playerCardX + cardWidth + 40;    // Posição X da carta da máquina

    // Posição para exibir os atributos da máquina
    int machineTextX = machineCardX + cardWidth + 20;

    // Dimensões e posições dos botões
    int buttonWidth = 200;                                                     // Largura dos botões
    int buttonHeight = 60;                                                     // Altura dos botões
    int buttonSpacing = 20;                                                    // Espaçamento entre os botões
    int totalButtonsWidth = (buttonWidth + buttonSpacing) * 4 - buttonSpacing; // Largura total dos botões
    int buttonX = (screenWidth - totalButtonsWidth) / 2;                       // Centraliza os botões
    int buttonY = screenHeight / 2 + cardHeight / 2 + 20;                      // Posição Y dos botões

    // Posições para exibir os atributos
    int textX = 20;    // Posição X para os atributos
    int textY = cardY; // Posição Y alinhada com a parte superior da carta

    // Carregamento de cartas
    int maxCartas = 32;             // Número máximo de cartas
    int jogoplay = 1;               // Controle de jogo
    char filename[] = "cartas.csv"; // Nome do arquivo CSV
    int contador = 0;               // Contador de linhas
    int coluna;                     // Contador de colunas
    int tamanhoBinario;             // Tamanho do arquivo binário
    char linhas[256];               // Buffer para ler linhas do arquivo
    int atributo = 0;               // Atributo selecionado
    int jogador_valor, bot_valor;   // Valores dos atributos do jogador e do bot
    int ganhador = -1;              // nem máquina nem player

    // Alocação de memória para as cartas do jogador e da máquina
    int tam_player = sizeof(Cartas) * (maxCartas / 2);
    int tam_maquina = sizeof(Cartas) * (maxCartas / 2);
    Cartas *Player = (Cartas *)malloc(tam_player);
    Cartas *Maquina = (Cartas *)malloc(tam_maquina);

    // Abertura dos arquivos
    FILE *binario = fopen("save.bin", "rb");
    FILE *file = fopen(filename, "r");

    // Alocação de memória para a lista de cartas
    Cartas *lista = (Cartas *)malloc(sizeof(Cartas) * maxCartas);
    if (lista == NULL)
    {
        printf("Não foi possível alocar dinâmicamente.\n");
        exit(1);
    }

    // Carregamento das cartas do arquivo CSV ou do arquivo binário
    if (binario == NULL)
    {
        if (file == NULL)
        {
            fprintf(stderr, "Não foi possível abrir o arquivo %s\n", filename);
            exit(1);
        }

        while (fgets(linhas, sizeof(linhas), file))
        {
            contador++;

            // Pula a linha do cabeçalho
            if (contador == 1)
            {
                continue;
            }

            // Preenche a estrutura com os dados do arquivo
            int index = contador - 2; // Índice no array
            if (index >= maxCartas)
            {
                break; // Garante que não ultrapasse o limite de cartas
            }

            // Processa cada coluna da linha
            char *token = strtok(linhas, ",");
            coluna = 0;
            while (token)
            {
                coluna++;
                switch (coluna)
                {
                case 2: // Nome
                    strncpy(lista[index].nome, token, sizeof(lista[index].nome) - 1);
                    lista[index].nome[sizeof(lista[index].nome) - 1] = '\0'; // Garante que a string esteja terminada
                    break;
                case 3:
                    lista[index].influencia = atoi(token); // Converte string para int
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
                    lista[index].letra = token[0]; // Armazena apenas o primeiro caractere
                    break;
                case 8:
                    lista[index].numero = atoi(token);
                    break;
                case 9:
                    lista[index].legado = atoi(token);
                    break;
                }
                token = strtok(NULL, ","); // Avança para o próximo token
            }
        }
    }
    else
    {
        printf("Bem-vindo de volta!\n");

        fseek(binario, 0, SEEK_END);                      // Move o ponteiro para o final do arquivo
        tamanhoBinario = ftell(binario);                  // Obtém o tamanho do arquivo
        int tamanho = tamanhoBinario / sizeof(Cartas);    // Calcula o número de cartas
        lista = realloc(lista, sizeof(Cartas) * tamanho); // Realoca a lista de cartas
        fseek(binario, 0, 0);                             // Move o ponteiro de volta para o início
        fread(lista, sizeof(Cartas), tamanho, binario);   // Lê as cartas do arquivo binário
        fclose(binario);                                  // Fecha o arquivo binário
    }

    fclose(file); // Fecha o arquivo CSV

    // Loop principal do jogo
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE); // Limpa o fundo
        DrawFPS(10, 10);        // Exibe a taxa de quadros

        // Tela do menu principal
        if (currentScreen == MAIN_MENU)
        {
            const char *title = "SuperTrunfo";                  // Título do jogo
            int fontSizeTitle = 100;                            // Tamanho da fonte do título
            int titleWidth = MeasureText(title, fontSizeTitle); // Largura do título
            int titleX = (screenWidth - titleWidth) / 2;        // Centraliza o título
            int titleY = screenHeight / 4;                      // Posição Y do título

            DrawText(title, titleX, titleY, fontSizeTitle, BLACK); // Desenha o título

            // Botão "Jogar"
            int buttonWidth = 300;
            int buttonHeight = 150;
            int buttonX = (screenWidth - buttonWidth) / 2; // Centraliza o botão
            int buttonY = titleY + fontSizeTitle + 20;     // Posição Y do botão

            Rectangle buttonBounds = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
            if (GuiButton(buttonBounds, "Jogar")) // Se o botão "Jogar" for pressionado
            {
                currentScreen = GAME; // Muda para a tela do jogo
            }

            // Botão "Conquistas"
            int achievementsButtonY = buttonY + buttonHeight + 20;
            buttonBounds.y = (float)achievementsButtonY;
            if (GuiButton(buttonBounds, "Conquistas")) // Se o botão "Conquistas" for pressionado
            {
                currentScreen = ACHIEVEMENTS; // Muda para a tela de conquistas
            }
        }
        // Tela do jogo
        else if (currentScreen == GAME)
        {
            if (tam_player == 0)
            {
                currentScreen = END_GAME;
                ganhador = MAQUINA;
            }

            // Embaralha e distribui as cartas na primeira vez que o jogo é iniciado
            if (jogoplay == 1)
            {
                distribuiCartas(lista, maxCartas, Player, Maquina); // Distribui as cartas
                jogoplay = 0;                                       // Altera o controle de jogo
            }

            // Texto acima das cartas
            DrawText("Sua Carta", playerCardX + cardWidth / 2 - MeasureText("Sua Carta", 20) / 2, cardY - 30, 20, BLACK);
            DrawText("Carta Máquina", machineCardX + cardWidth / 2 - MeasureText("Carta Máquina", 20) / 2, cardY - 30, 20, BLACK);

            // Desenho das cartas
            DrawTexture(cartaPlayer, playerCardX, cardY, WHITE);                     // Desenha a carta do jogador
            DrawRectangleLines(playerCardX, cardY, cardWidth, cardHeight, DARKGRAY); // Desenha a borda da carta do jogador

            DrawTexture(cartaMaquina, machineCardX, cardY, WHITE);                    // Desenha a carta da máquina
            DrawRectangleLines(machineCardX, cardY, cardWidth, cardHeight, DARKGRAY); // Desenha a borda da carta da máquina

            // Desenho dos atributos da carta do jogador
            DrawText("Atributos da Sua Carta:", textX, textY, 20, BLACK);
            DrawText(TextFormat("Nome: %s", Player[0].nome), textX, textY + 30, 20, BLACK);
            DrawText(TextFormat("Influência: %d", Player[0].influencia), textX, textY + 60, 20, BLACK);
            DrawText(TextFormat("Estratégia: %d", Player[0].estrategia), textX, textY + 90, 20, BLACK);
            DrawText(TextFormat("Popularidade: %d", Player[0].popularidade), textX, textY + 120, 20, BLACK);
            DrawText(TextFormat("Legado: %d", Player[0].legado), textX, textY + 150, 20, BLACK);
            DrawText(TextFormat("Código: %c-%d", Player[0].letra, Player[0].numero), textX, textY + 180, 20, BLACK);

            // Botões para selecionar atributos
            if (GuiButton((Rectangle){buttonX, buttonY, buttonWidth, buttonHeight}, "Influência"))
            {
                atributo = 1; // Define o atributo para influência
            }
            if (GuiButton((Rectangle){buttonX + (buttonWidth + buttonSpacing) * 1, buttonY, buttonWidth, buttonHeight}, "Estratégia"))
            {
                atributo = 2; // Define o atributo para estratégia
            }
            if (GuiButton((Rectangle){buttonX + (buttonWidth + buttonSpacing) * 2, buttonY, buttonWidth, buttonHeight}, "Popularidade"))
            {
                atributo = 3; // Define o atributo para popularidade
            }
            if (GuiButton((Rectangle){buttonX + (buttonWidth + buttonSpacing) * 3, buttonY, buttonWidth, buttonHeight}, "Legado"))
            {
                atributo = 4; // Define o atributo para legado
            }

            // Verifica qual atributo foi selecionado
            if (atributo != 0)
            {
                switch (atributo)
                {
                case 1:
                    jogador_valor = Player[0].influencia; // Atributo do jogador
                    bot_valor = Maquina[0].influencia;    // Atributo do bot
                    break;
                case 2:
                    jogador_valor = Player[0].estrategia;
                    bot_valor = Maquina[0].estrategia;
                    break;
                case 3:
                    jogador_valor = Player[0].popularidade;
                    bot_valor = Maquina[0].popularidade;
                    break;
                case 4:
                    jogador_valor = Player[0].legado;
                    bot_valor = Maquina[0].legado;
                    break;
                }
                if (Player[0].super_trunfo == 1)
                {
                    // e caso maquina tenha A
                    if (Maquina[0].letra == 'A' || Maquina[0].letra == 'a')
                    {
                        currentScreen = GAME_Maquina;
                    }
                    else
                    {
                        // Maquina não tem A
                        currentScreen = GAME_Player;
                    }
                    // caso bot tenha supertrunfo
                }
                else if (Maquina[0].super_trunfo == 1)
                {
                    // e caso player tenha A
                    if (Player[0].letra == 'a' || Player[0].letra == 'A')
                    {
                        currentScreen = GAME_Player;
                    }
                    else
                    {
                        // caso player não tenha A
                        currentScreen = GAME_Maquina;
                    }
                }
                else
                    // caso de atributos normais
                    if (jogador_valor > bot_valor)
                    {
                        // jogador com atributo maior
                        currentScreen = GAME_Player;
                    }
                    else
                    {
                        // maquina com atributo maior
                        currentScreen = GAME_Maquina;
                    }
            }
            // caso player tenha supertrunfo

            // Botão de voltar
            Rectangle backButtonBounds = {1050, 20, 150, 60};
            if (GuiButton(backButtonBounds, "Voltar"))
            {
                previousScreen = GAME;        // Armazena a tela anterior
                currentScreen = CONFIRM_EXIT; // Muda para a tela de confirmação
            }
        }

        // Tela qndo maquina quanha rodada
        if (currentScreen == GAME_Player)
        {
            // Texto acima das cartas
            DrawText("Sua Carta", playerCardX + cardWidth / 2 - MeasureText("Sua Carta", 20) / 2, cardY - 30, 20, BLACK);
            DrawText("Carta Máquina", machineCardX + cardWidth / 2 - MeasureText("Carta Máquina", 20) / 2, cardY - 30, 20, BLACK);

            // Desenho das cartas
            DrawTexture(cartaPlayer, playerCardX, cardY, WHITE);                     // Desenha a carta do jogador
            DrawRectangleLines(playerCardX, cardY, cardWidth, cardHeight, DARKGRAY); // Desenha a borda da carta do jogador

            DrawTexture(cartaMaquina, machineCardX, cardY, WHITE);                    // Desenha a carta da máquina
            DrawRectangleLines(machineCardX, cardY, cardWidth, cardHeight, DARKGRAY); // Desenha a borda da carta da máquina

            // Exibe os atributos da carta do jogador
            DrawText("Atributos da Sua Carta:", textX, textY, 20, BLACK);
            DrawText(TextFormat("Nome: %s", Player[0].nome), textX, textY + 30, 20, BLACK);
            DrawText(TextFormat("Influência: %d", Player[0].influencia), textX, textY + 60, 20, BLACK);
            DrawText(TextFormat("Estratégia: %d", Player[0].estrategia), textX, textY + 90, 20, BLACK);
            DrawText(TextFormat("Popularidade: %d", Player[0].popularidade), textX, textY + 120, 20, BLACK);
            DrawText(TextFormat("Legado: %d", Player[0].legado), textX, textY + 150, 20, BLACK);
            DrawText(TextFormat("Código: %c-%d", Player[0].letra, Player[0].numero), textX, textY + 180, 20, BLACK);

            // Exibe os atributos da carta da máquina
            DrawText("Atributos da Carta da Máquina:", machineTextX, textY, 20, BLACK);
            DrawText(TextFormat("Nome: %s", Maquina[0].nome), machineTextX, textY + 30, 20, BLACK);
            DrawText(TextFormat("Influência: %d", Maquina[0].influencia), machineTextX, textY + 60, 20, BLACK);
            DrawText(TextFormat("Estratégia: %d", Maquina[0].estrategia), machineTextX, textY + 90, 20, BLACK);
            DrawText(TextFormat("Popularidade: %d", Maquina[0].popularidade), machineTextX, textY + 120, 20, BLACK);
            DrawText(TextFormat("Legado: %d", Maquina[0].legado), machineTextX, textY + 150, 20, BLACK);
            DrawText(TextFormat("Código: %c-%d", Maquina[0].letra, Maquina[0].numero), machineTextX, textY + 180, 20, BLACK);

            // mensagem vitoria
            const char text[] = "voce ganhou a rodada!";
            int fontSize = 40;
            int posY = 20;
            int textWidth = MeasureText(text, fontSize);
            int posX = (1200 - textWidth) / 2;
            DrawText(TextFormat("%s", text), posX, posY, fontSize, BLACK);

            // Botão para próxima rodada
            if (GuiButton((Rectangle){buttonX, buttonY + buttonHeight, buttonWidth * 2, buttonHeight}, "Próxima Rodada"))
            {
   /*
                // coloca a carta do perdedor no final do baralho do ganhador
                Player = realloc(Player, (tam_player + 1) * sizeof(Cartas));
                Player[tam_player] = Maquina[0];
                tam_player++;
                memmove(Maquina, Maquina + 1, (tam_maquina - 1) * sizeof(Cartas));
                (tam_maquina)--;
                Maquina = realloc(Maquina, tam_maquina * sizeof(Cartas));
   */
                atributo = 0;         // Redefine atributo
                currentScreen = GAME; // Volta para a tela do jogo
            }

            // Botão de voltar
            Rectangle backButtonBounds = {1050, 20, 150, 60};
            if (GuiButton(backButtonBounds, "Voltar"))
            {
                previousScreen = GAME_Maquina; // Armazena a tela anterior
                currentScreen = CONFIRM_EXIT;  // Muda para a tela de confirmação
            }
        }

        // Tela qndo maquina quanha rodada
        if (currentScreen == GAME_Maquina)
        {
            // Texto acima das cartas
            DrawText("Sua Carta", playerCardX + cardWidth / 2 - MeasureText("Sua Carta", 20) / 2, cardY - 30, 20, BLACK);
            DrawText("Carta Máquina", machineCardX + cardWidth / 2 - MeasureText("Carta Máquina", 20) / 2, cardY - 30, 20, BLACK);

            // Desenho das cartas
            DrawTexture(cartaPlayer, playerCardX, cardY, WHITE);                     // Desenha a carta do jogador
            DrawRectangleLines(playerCardX, cardY, cardWidth, cardHeight, DARKGRAY); // Desenha a borda da carta do jogador

            DrawTexture(cartaMaquina, machineCardX, cardY, WHITE);                    // Desenha a carta da máquina
            DrawRectangleLines(machineCardX, cardY, cardWidth, cardHeight, DARKGRAY); // Desenha a borda da carta da máquina

            // Exibe os atributos da carta do jogador
            DrawText("Atributos da Sua Carta:", textX, textY, 20, BLACK);
            DrawText(TextFormat("Nome: %s", Player[0].nome), textX, textY + 30, 20, BLACK);
            DrawText(TextFormat("Influência: %d", Player[0].influencia), textX, textY + 60, 20, BLACK);
            DrawText(TextFormat("Estratégia: %d", Player[0].estrategia), textX, textY + 90, 20, BLACK);
            DrawText(TextFormat("Popularidade: %d", Player[0].popularidade), textX, textY + 120, 20, BLACK);
            DrawText(TextFormat("Legado: %d", Player[0].legado), textX, textY + 150, 20, BLACK);
            DrawText(TextFormat("Código: %c-%d", Player[0].letra, Player[0].numero), textX, textY + 180, 20, BLACK);

            // Exibe os atributos da carta da máquina
            DrawText("Atributos da Carta da Máquina:", machineTextX, textY, 20, BLACK);
            DrawText(TextFormat("Nome: %s", Maquina[0].nome), machineTextX, textY + 30, 20, BLACK);
            DrawText(TextFormat("Influência: %d", Maquina[0].influencia), machineTextX, textY + 60, 20, BLACK);
            DrawText(TextFormat("Estratégia: %d", Maquina[0].estrategia), machineTextX, textY + 90, 20, BLACK);
            DrawText(TextFormat("Popularidade: %d", Maquina[0].popularidade), machineTextX, textY + 120, 20, BLACK);
            DrawText(TextFormat("Legado: %d", Maquina[0].legado), machineTextX, textY + 150, 20, BLACK);
            DrawText(TextFormat("Código: %c-%d", Maquina[0].letra, Maquina[0].numero), machineTextX, textY + 180, 20, BLACK);
            // mensagem de vitoria
            const char text[] = "Maquina ganhou a rodada!";
            int fontSize = 40;
            int posY = 20;
            int textWidth = MeasureText(text, fontSize);
            int posX = (1200 - textWidth) / 2;
            DrawText(TextFormat("%s", text), posX, posY, fontSize, BLACK);

            // Botão para próxima rodada
            if (GuiButton((Rectangle){buttonX, buttonY + buttonHeight, buttonWidth * 2, buttonHeight}, "Próxima Rodada"))
            {
 /*
                // coloca a carta do perdedor no final do baralho do ganhador
                Maquina = realloc(Maquina, (tam_maquina + 1) * sizeof(Cartas));
                Maquina[tam_maquina] = Player[0];
                tam_maquina++;
                memmove(Player, Player + 1, (tam_player - 1) * sizeof(Cartas));
                tam_player--;
                Player = realloc(Player, tam_player * sizeof(Cartas));
 */    
                atributo = 0;         // Redefine atributo
                currentScreen = GAME; // Volta para a tela do jogo
            }

            // Botão de voltar
            Rectangle backButtonBounds = {1050, 20, 150, 60};
            if (GuiButton(backButtonBounds, "Voltar"))
            {
                previousScreen = GAME_Player; // Armazena a tela anterior
                currentScreen = CONFIRM_EXIT; // Muda para a tela de confirmação
            }
        }
        // Tela de confirmação de saída
        else if (currentScreen == CONFIRM_EXIT)
        {
            // Tela de confirmação
            DrawRectangle(300, 300, 600, 200, LIGHTGRAY);
            DrawRectangleLines(300, 300, 600, 200, DARKGRAY);
            DrawText("Tem certeza que deseja voltar?", 400, 350, 20, BLACK);

            // Botões de confirmação
            if (GuiButton((Rectangle){400, 450, 100, 50}, "Sim"))
            {
                currentScreen = MAIN_MENU; // Volta para o menu principal
                jogoplay = 1;              // Reinicia o controle de jogo
            }
            if (GuiButton((Rectangle){700, 450, 100, 50}, "Não"))
            {
                currentScreen = previousScreen; // Retorna para a tela anterior
            }
        }

        EndDrawing(); // Finaliza o desenho
    }

    // Libera as texturas e fecha a janela
    UnloadTexture(cartaPlayer);
    UnloadTexture(cartaMaquina);
    CloseWindow();

    return 0; // Finaliza o programa
}
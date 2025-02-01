#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Define o MAX_CARTAS da tela
#define WIDTH 1200
#define HEIGHT 1000

// Define as posições dos elementos na tela
#define LOGO_X 180
#define LOGO_Y 137

#define SUBTITLE_X 422
#define SUBTITLE_Y 320
#define BUTTON_WIDTH 464
#define BUTTON_HEIGHT 97
#define BUTTON_POS_X 366
#define BUTTON_POS_Y 458
#define SPACE BUTTON_HEIGHT + 50
#define JOGAR_X 488
#define JOGAR_Y 462
#define CONQUISTAS_X 372
#define CONQUISTAS_Y 612
#define DECK_X 507
#define DECK_Y 758
#define BACKGROUND_X 0
#define BACKGROUND_Y 0

// Define os caminhos para os elementos
#define LOGO_PATH "resources/elementos/SuperTrunfo.png"
#define SUBTITLE_PATH "resources\\elementos\\historico.png"
#define BACKGROUND_PATH "resources\\Elementos\\imagemfundo.png"
#define LOGO_PATHPP "resources\\elementos\\Group-2.png"
#define TOP_PATH "resources\\elementos\\Group.png"
#define CONFIRME_PATH "resources\\elementos\\temcerteza.png"
#define PLAYERWIN_PATH "resources\\elementos\\playerganha.png"
#define BOTRWIN_PATH "resources\\elementos\\botganha.png"
#define CONQUISTA_PATH "resources\\elementos\\conquista.png"
#define CAPA_PATH "resources\\cartas\\capa.png"

// telas do jogo
typedef enum
{
    MAIN_MENU,
    GAME,
    ACHIEVEMENTS,
    CONFIRM_EXIT,
    GAME_BOT,
    GAME_PLAYER,
    GAME_EMPATE,
    END_GAME,
    TELACONQUISTA,
    PLAYER_WIN,
    BOT_WIN,

} Telas;

// Enum para determinar quem joga no momento
typedef enum
{
    EMPATE = -1,
    PLAYER,
    BOT

} Rodada;

// atributos
typedef enum
{
    INFLUENCIA = 1,
    ESTRATEGIA,
    POPULARIDADE,
    LEGADO
} Atributos;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// funções de cartas que deveriam estar em outro arquivo

// Estrutura que representa uma carta
typedef struct
{
    char nome[40];    // Nome da carta
    char letra;       // Letra identificadora da carta
    char numero;      // Número da carta
    int super_trunfo; // Indica se é um super trunfo
    int influencia;   // Atributo de influência
    int estrategia;   // Atributo de estratégia
    int popularidade; // Atributo de popularidade
    int legado;       // Atributo de legado
} Cartas;

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
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Cartas temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Function to distribute Cartas into Cartas_jogador and Cartas_bot
void distribuiCartas(Cartas *array, int n, Cartas *jogador, Cartas *bot)
{
    shuffleCartas(array, n);
    int metade = n / 2;
    if (n % 2 != 0)
    {
        printf("Número de cartas ímpar, impossível distribuir igualmente.\n");
        return;
    }
    for (int i = 0; i < metade; i++)
    {
        strcpy(jogador[i].nome, array[i].nome);
        jogador[i].letra = array[i].letra;
        jogador[i].numero = array[i].numero;
        jogador[i].super_trunfo = array[i].super_trunfo;
        jogador[i].influencia = array[i].influencia;
        jogador[i].estrategia = array[i].estrategia;
        jogador[i].popularidade = array[i].popularidade;
        jogador[i].legado = array[i].legado;
    }
    for (int i = metade; i < n; i++)
    {
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

// gerar uma imagem
void geraImagemCarta(Texture2D *carta, Cartas cartaInfo)
{
    char filePath[] = "resources\\cartas\\";
    printf("%s\n", filePath);

    int i = cartaInfo.numero;
    char b[2];
    b[0] = cartaInfo.letra;
    b[1] = '\0';
    char buffer[33];
    printf("%i - %c", i, b[0]);

    printf("file: %s\n", filePath);

    strcat(filePath, b);

    printf("file: %s\n", filePath);

    itoa(i, buffer, 10);

    strcat(filePath, buffer);

    strcat(filePath, ".png");
    printf("file: %s\n", filePath);

    // Carrega a imagem
    Image carta_i = LoadImage(filePath);

    // Verifica se a imagem foi carregada corretamente
    if (carta_i.data == NULL)
    {
        carta_i = LoadImage(CAPA_PATH);
    }

    // Redimensiona a imagem
    ImageResize(&carta_i, 300, 450);

    // Carrega a textura a partir da imagem
    *carta = LoadTextureFromImage(carta_i);

    // Libera a imagem da memória
    UnloadImage(carta_i);
}

// comparar atributos
int DeterminarGanhador(int jogador_valor, int bot_valor, Cartas *Player, Cartas *Bot)
{

    // determinar quem ganhou a rodada
    // caso o jogador tem supertrunfo
    if (Player[0].super_trunfo == 1)
    {
        // caso bot tem a
        if (Bot[0].letra == 'A' || Bot[0].letra == 'a')
        {
            return BOT;
        }
        // caso bot não tem a
        else
        {
            return PLAYER;
        }
    }

    // caso bot tem supertrunfo
    else if (Bot[0].super_trunfo == 1)
    {
        // caso jogador tem a
        if (Player[0].letra == 'A' || Player[0].letra == 'a')
        {
            return PLAYER;
        }
        else
        {
            // caso jogador não tem a
            return BOT;
        }
    }

    // caso ninguém tem supertrunfo
    else
    {
        if (jogador_valor > bot_valor)
        {
            return PLAYER;
        }
        else if (jogador_valor < bot_valor)
        {
            return BOT;
        }
        else
        {
            return EMPATE;
        }
    }
}

int main(void)
{
    int conquistas[3] = {0, 0, 0};

    // Carregamento de cartas
    const int MAX_CARTAS = 32;
    int jogoplay = 0; // marca se o jogo começou ou não
    char filename[] = "cartas.csv";
    int contador = 0;
    int coluna;
    int tamanhoBinario;
    char linhas[256];
    int atributo = 0;
    int jogador_valor, bot_valor;
    int ganhador = -1;
    int rodada = 0;

    int totalCartasBot = MAX_CARTAS / 2;
    int totalCartasPlayer = MAX_CARTAS / 2;

    // Alocação de memória para as cartas do jogador e da máquina
    Cartas *baralho = (Cartas *)malloc(sizeof(Cartas) * MAX_CARTAS);
    Cartas *Player = (Cartas *)malloc(sizeof(Cartas) * (totalCartasPlayer));
    Cartas *Bot = (Cartas *)malloc(sizeof(Cartas) * (totalCartasBot));

    if (baralho == NULL)
    {
        printf("Não foi possível alocar dinâmicamente.\n");
        exit(1);
    }

    // Abertura dos arquivos
    FILE *binario = fopen("save.bin", "rb");
    FILE *file = fopen(filename, "r");

    if (binario == NULL)
    {
        if (file == NULL)
        {
            fprintf(stderr, "Não foi possivel abrir o arquivo %s\n", filename);
            exit(1);
        }

        printf("Seja bem-vindo!\n");

        while (fgets(linhas, sizeof(linhas), file))
        {
            contador++;

            // Pula a linha do cabeçalho
            if (contador == 1)
            {
                continue;
            }

            // Preenche a estrutura
            int index = contador - 2; // Índice no array
            if (index >= MAX_CARTAS)
            {
                break; // Garante que não ultrapasse o limite de n cartas, no caso 10
            }

            // Casos tokens
            char *token = strtok(linhas, ",");
            coluna = 0;
            while (token)
            {
                coluna++;
                switch (coluna)
                {
                case 2: // Nome
                    strncpy(baralho[index].nome, token, sizeof(baralho[index].nome) - 1);
                    baralho[index].nome[sizeof(baralho[index].nome) - 1] = '\0';
                    break;
                case 3:
                    baralho[index].influencia = atoi(token); // Salvo como int (converte string -> int)
                    break;
                case 4:
                    baralho[index].estrategia = atoi(token);
                    break;
                case 5:
                    baralho[index].popularidade = atoi(token);
                    break;
                case 6:
                    baralho[index].super_trunfo = atoi(token);
                    break;
                case 7:
                    baralho[index].letra = token[0]; // Corrige para armazenar apenas o primeiro caractere
                    break;
                case 8:
                    baralho[index].numero = atoi(token);
                    break;
                case 9:
                    baralho[index].legado = atoi(token);
                    break;
                }
                token = strtok(NULL, ",");
            }
        }
    }
    else
    {
        printf("Bem vindo de volta!\n");

        fseek(binario, 0, SEEK_END);
        tamanhoBinario = ftell(binario);
        tamanhoBinario = tamanhoBinario / sizeof(Cartas);
        baralho = realloc(baralho, sizeof(Cartas) * MAX_CARTAS);
        fseek(binario, 0, 0);
        fread(baralho, sizeof(Cartas), MAX_CARTAS, binario);
        fclose(binario);
    }

    fclose(file);
    //   ----------------------------- EDITOR DE BARALHO --------------------------------

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Inicializa
    InitWindow(WIDTH, HEIGHT, "Super Trunfo Histórico");

    // Carrega os elementos
    Texture2D logo = LoadTexture(LOGO_PATH);
    Texture2D background = LoadTexture(BACKGROUND_PATH);
    Texture2D subtitulo = LoadTexture(SUBTITLE_PATH);
    Texture2D logopp = LoadTexture(LOGO_PATHPP);
    Texture2D top = LoadTexture(TOP_PATH);
    Texture2D confirme = LoadTexture(CONFIRME_PATH);
    Texture2D playerwin = LoadTexture(PLAYERWIN_PATH);
    Texture2D botwin = LoadTexture(BOTRWIN_PATH);
    Texture2D conquista = LoadTexture(CONQUISTA_PATH);
    Texture2D capa = LoadTexture(CAPA_PATH);

    // inicializa cartas
    Texture2D cartaPlayer;
    Texture2D cartaBot;

    // define o estilo inicial ao abrir o jogo
    GuiLoadStyle("resources\\Elementos\\antigo.rgs");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 75);

    // define as telas para controle
    int currentScreen = MAIN_MENU;
    int previousScreen = MAIN_MENU;

    // Loop principal do jogo
    while (!WindowShouldClose())
    {

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // TELA MENU
        if (currentScreen == MAIN_MENU)
        {

            // Desenha o fundo
            DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
            BeginDrawing();
            ClearBackground(RAYWHITE);
            // Desenha o subtitulo
            DrawTexture(subtitulo, SUBTITLE_X, SUBTITLE_Y, WHITE);
            // Desenha o logo
            DrawTexture(logo, LOGO_X, LOGO_Y, WHITE);

            // Desenha os botões
            if (GuiButton((Rectangle){BUTTON_POS_X, BUTTON_POS_Y, BUTTON_WIDTH, BUTTON_HEIGHT}, "JOGAR"))
            {
                // Ação para o botão JOGAR
                printf("Botão JOGAR pressionado!\n");
                currentScreen = GAME;
            }
            if (GuiButton((Rectangle){BUTTON_POS_X, BUTTON_POS_Y + 1 * (SPACE), BUTTON_WIDTH, BUTTON_HEIGHT}, "CONQUISTAS"))
            {
                // Ação para o botão CONQUISTAS
                printf("Botão CONQUISTAS pressionado!\n");
            }
            if (GuiButton((Rectangle){BUTTON_POS_X, BUTTON_POS_Y + 2 * (SPACE), BUTTON_WIDTH, BUTTON_HEIGHT}, "DECK"))
            {
                // Ação para o botão DECK
                printf("Botão DECK pressionado!\n");
            }
        } // FIM DO MAIN_MENU

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // TELAS DE JOGO!
        //         ----------------------- ESCOLHA ATRIBUTOS -----------------------
        if (currentScreen == GAME)
        {

            // caso primeiro jogo
            if (jogoplay == 0)
            {
                distribuiCartas(baralho, MAX_CARTAS, Player, Bot);
                jogoplay = 1; // começou um jogo
            }

            // rodada
            if (rodada == 0)
            {
                // gerar cartas atuais
                geraImagemCarta(&cartaPlayer, Player[0]);
                geraImagemCarta(&cartaBot, Bot[0]);
                rodada = 1;
                atributo = 0;
            }

            // Desenha o fundo
            {
                DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
                ClearBackground(RAYWHITE);

                DrawTexture(logopp, 0, 0, WHITE);
                DrawTexture(top, 163, 121, WHITE);

                DrawTexture(cartaPlayer, 203, 349, WHITE);
                DrawTexture(capa, 692, 349, WHITE);
            }

            // Desenha os botões de atributos
            GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
            {
                if (GuiButton((Rectangle){39, 845, 253, 57}, "Influência"))
                {
                    atributo = INFLUENCIA;
                    jogador_valor = Player[0].influencia;
                    bot_valor = Bot[0].influencia;
                }
                if (GuiButton((Rectangle){39 + 1 * (35 + 253), 845, 253, 57}, "Estratégia"))
                {
                    atributo = ESTRATEGIA;
                    jogador_valor = Player[0].estrategia;
                    bot_valor = Bot[0].estrategia;
                }
                if (GuiButton((Rectangle){39 + 2 * (35 + 253), 845, 253, 57}, "Popularidade"))
                {
                    atributo = POPULARIDADE;
                    jogador_valor = Player[0].popularidade;
                    bot_valor = Bot[0].popularidade;
                }
                if (GuiButton((Rectangle){39 + 3 * (35 + 253), 845, 253, 57}, "Legado"))
                {
                    atributo = LEGADO;
                    jogador_valor = Player[0].legado;
                    bot_valor = Bot[0].legado;
                }
            } // botoes de atributo

            if (atributo != 0)
            {
                ganhador = DeterminarGanhador(jogador_valor, bot_valor, Player, Bot);
                if (ganhador == PLAYER)
                {
                    currentScreen = GAME_PLAYER;
                    if (Player[0].super_trunfo == 1)
                    {
                        currentScreen = TELACONQUISTA;
                    }
                }
                else if (ganhador == BOT)
                {
                    currentScreen = GAME_BOT;
                }
                else if (ganhador == EMPATE)
                {
                    currentScreen = GAME_EMPATE;
                }
            }

            // botão saida
            if (GuiButton((Rectangle){1140, 10, 50, 50}, "X"))
            {
                previousScreen = GAME;
                currentScreen = CONFIRM_EXIT;
            }
        }

        //         ----------------------- CASO PLAYER GANHE A RODADA -----------------------
        if (currentScreen == GAME_PLAYER)
        {

            // Desenha o fundo
            {
                GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
                DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
                ClearBackground(RAYWHITE);

                DrawTexture(logopp, 0, 0, WHITE);
                DrawTexture(top, 163, 121, WHITE);

                DrawTexture(cartaPlayer, 203, 349, WHITE);
                DrawTexture(cartaBot, 692, 349, WHITE);
                DrawTexture(playerwin, 503, 920, WHITE);
            }

            // botão saida + proxima rodada
            {

                // saida
                if (GuiButton((Rectangle){1140, 10, 50, 50}, "X"))
                {
                    previousScreen = GAME_PLAYER;
                    currentScreen = CONFIRM_EXIT;
                }

                // botão proxima rodada
                if (GuiButton((Rectangle){39, 920, 300, 57}, "Próxima Rodada"))
                {
                    Player = realloc(Player, (totalCartasPlayer + 2) * sizeof(Cartas));
                    (Player)[totalCartasPlayer] = (Bot)[0];
                    (Player)[totalCartasPlayer + 1] = (Player)[0];
                    (totalCartasPlayer) += 2;

                    memmove(Player, Player + 1, (totalCartasPlayer - 2) * sizeof(Cartas));
                    (totalCartasPlayer)--;
                    Player = realloc(Player, totalCartasPlayer * sizeof(Cartas));

                    memmove(Bot, Bot + 1, (totalCartasBot - 1) * sizeof(Cartas));
                    (totalCartasBot)--;
                    Bot = realloc(Bot, totalCartasBot * sizeof(Cartas));

                    atributo = 0;
                    rodada = 0;
                    
                    if (totalCartasBot == 0)
                    {
                        currentScreen = PLAYER_WIN;
                    }
                    else
                    {
                        currentScreen = GAME;
                    }
                }
            }
        } // fim do game_player

        //         ----------------------- CASO QNT DE CARTAS BOT ACABE (PLAYER GANHA JOGO) -----------------------
        if (currentScreen == PLAYER_WIN)
        {
            DrawText("Vitória Player", 300, 300, 50, WHITE);
        }

        //         ----------------------- CASO BOT GANHE A RODADA  -----------------------
        if (currentScreen == GAME_BOT)
        {

            // Desenha o fundo
            {
                GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
                DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
                ClearBackground(RAYWHITE);

                DrawTexture(logopp, 0, 0, WHITE);
                DrawTexture(top, 163, 121, WHITE);

                DrawTexture(cartaPlayer, 203, 349, WHITE);
                DrawTexture(cartaBot, 692, 349, WHITE);
                DrawTexture(botwin, 503, 920, WHITE);
            }

            // botão saida + proxima rodada
            {

                if (GuiButton((Rectangle){1140, 10, 50, 50}, "X"))
                {
                    previousScreen = GAME_BOT;
                    currentScreen = CONFIRM_EXIT;
                }

                if (GuiButton((Rectangle){39, 920, 300, 57}, "Próxima Rodada"))
                {

                    Bot = realloc(Bot, (totalCartasBot + 2) * sizeof(Cartas));
                    (Bot)[totalCartasBot] = (Player)[0];
                    (Bot)[totalCartasBot + 1] = (Bot)[0];
                    (totalCartasBot) += 2;

                    memmove(Player, Player + 1, (totalCartasPlayer - 1) * sizeof(Cartas));
                    (totalCartasPlayer)--;
                    Player = realloc(Player, totalCartasPlayer * sizeof(Cartas));

                    memmove(Bot, Bot + 1, (totalCartasBot - 2) * sizeof(Cartas));
                    (totalCartasBot)--;
                    Bot = realloc(Bot, totalCartasBot * sizeof(Cartas));

                    atributo = 0;
                    rodada = 0;
                    if (totalCartasPlayer == 0)
                    {
                        currentScreen = BOT_WIN;
                    }
                    else
                    {
                        currentScreen = GAME;
                    }
                }
            }
        } // fim do game_bot

        //         ----------------------- CASO QNT DE CARTAS JOGADOR ACABE (PLAYER GANHA BOT) -----------------------
        if (currentScreen == BOT_WIN)
        {
            DrawText("Vitória Bot", 300, 300, 50, WHITE);
        }

        //         ----------------------- CASO EMPATE RODADA -----------------------
        if (currentScreen == GAME_EMPATE)
        {
            {
                GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
                DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
                ClearBackground(RAYWHITE);

                DrawTexture(logopp, 0, 0, WHITE);
                DrawTexture(top, 163, 121, WHITE);

                DrawTexture(cartaPlayer, 203, 349, WHITE);
                DrawTexture(cartaBot, 692, 349, WHITE);
                // DrawTexture(botwin, 503, 920, WHITE);
            }

            if (GuiButton((Rectangle){39, 920, 300, 57}, "Próxima Rodada"))
            {
                Player = realloc(Player, (totalCartasPlayer + 2) * sizeof(Cartas));
                (Player)[totalCartasPlayer] = (Bot)[0];
                (Player)[totalCartasPlayer + 1] = (Player)[0];
                (totalCartasPlayer) += 2;

                memmove(Player, Player + 1, (totalCartasPlayer - 2) * sizeof(Cartas));
                (totalCartasPlayer)--;
                Player = realloc(Player, totalCartasPlayer * sizeof(Cartas));

                memmove(Bot, Bot + 1, (totalCartasBot - 1) * sizeof(Cartas));
                (totalCartasBot)--;
                Bot = realloc(Bot, totalCartasBot * sizeof(Cartas));

                atributo = 0;
                rodada = 0;
                currentScreen = GAME;
            }
        }

        //------------------------------ TELAS DE AVISOS DE CONQUISTAS ----------------------
        if (currentScreen == TELACONQUISTA)
        {
            if (Player[0].super_trunfo == 1 && !(Bot[0].letra == 'a' || Bot[0].letra == 'A'))
            {
                DrawRectangle(350, 300, 500, 200, WHITE);
                DrawRectangleLines(350, 300, 500, 200, WHITE);
                DrawTexture(conquista, 438, 340, WHITE);

                if (GuiButton((Rectangle){450, 430, 300, 50}, "Continuar"))
                {
                    currentScreen = GAME_PLAYER;
                }
            }
        } // fim das telas de conquista

        //------------------------- TELA DE CONFIRMAR SAIDA -------------------------//
        if (currentScreen == CONFIRM_EXIT)
        {

            DrawRectangle(350, 300, 500, 200, WHITE);
            DrawRectangleLines(350, 300, 500, 200, WHITE);
            DrawTexture(confirme, 438, 340, WHITE);

            if (GuiButton((Rectangle){400, 430, 100, 50}, "Sim"))
            {
                currentScreen = MAIN_MENU;
                previousScreen = MAIN_MENU;
                jogoplay = 0;
                rodada = 0;
                atributo = 0;
                UnloadTexture(cartaPlayer);
                UnloadTexture(cartaBot);
                GuiSetStyle(DEFAULT, TEXT_SIZE, 75);
            }
            if (GuiButton((Rectangle){700, 430, 100, 50}, "Não"))
            {
                currentScreen = previousScreen;
            }
        } // fim do confirm_exit

        EndDrawing();
    } // whille (WindowShouldClose)

    // Unload textures
    UnloadTexture(logo);
    UnloadTexture(subtitulo);
    UnloadTexture(background);

    // Fecha a janela
    CloseWindow();

    return 0;
}
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FRAME_DELAY 20
#define MIN_FRAME_DELAY 1

// Define o tamanho da tela
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
#define CAPA_PATH "resources\\cartas\\capa.png"
#define CONQUISTA_PATH "resources\\elementos\\conquista.png"
#define EMPATE_PATH "resources\\elementos\\empate.png"
#define MAGO_PATH "resources\\elementos\\mago.png"
#define NOMAGO_PATH "resources\\elementos\\nomago.png"
#define INTERROGACAO_PATH "resources\\elementos\\interrogacao.png"

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
    PROMPTCONQUISTAS,
    PLAYER_WIN,
    BOT_WIN,

} Telas;

// Enum para determinar quem joga no momento
typedef enum
{
    EMPATE,
    PLAYER,
    BOT,
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

// Função para distribuir as cartas entre o jogador e o bot
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
        jogador[i] = array[i];
    }
    for (int i = metade; i < n; i++)
    {
        bot[i - metade] = array[i];
    }
}

// gerar uma imagem
void geraImagemCarta(Texture2D *carta, Cartas cartaInfo)
{
    char filePath[] = "resources\\cartas\\";

    int i = cartaInfo.numero;
    char b[2];
    b[0] = cartaInfo.letra;
    b[1] = '\0';
    char buffer[33];

    strcat(filePath, b);
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

// comparar atributos
int DeterminarGanhador(int jogador_valor, int maquina_valor, Cartas *Player, Cartas *Bot)
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
        if (jogador_valor > maquina_valor)
        {
            return PLAYER;
        }
        else if (jogador_valor < maquina_valor)
        {
            return BOT;
        }
        else if (jogador_valor == maquina_valor)
        {
            return EMPATE;
        }
    }
}

void AtualizaDeck(Cartas **jogador, int *tam_jogador, Cartas **bot, int *tam_bot, int currentScreen)
{

    // Move cartas para os respectivos decks
    Cartas carta_jogador = (*jogador)[0];
    Cartas carta_bot = (*bot)[0];

    // Atualiza o deck do jogador
    memmove(*jogador, *jogador + 1, (*tam_jogador - 1) * sizeof(Cartas));
    *tam_jogador = *tam_jogador - 1;
    printf("- JOGADOR\n");
    *jogador = realloc(*jogador, (*tam_jogador) * sizeof(Cartas));

    // Atualiza o deck do bot
    memmove(*bot, *bot + 1, (*tam_bot - 1) * sizeof(Cartas));
    *tam_bot = *tam_bot - 1;
    printf("- BOT\n");
    *bot = realloc(*bot, (*tam_bot) * sizeof(Cartas));

    if (currentScreen == GAME_PLAYER)
    {
        *jogador = realloc(*jogador, (*tam_jogador + 2) * sizeof(Cartas));
        (*jogador)[*tam_jogador] = carta_jogador;
        (*jogador)[*tam_jogador + 1] = carta_bot;
        *tam_jogador = *tam_jogador + 2;
        printf(" + 2 JOGADOR\n");
    }

    if (currentScreen == GAME_BOT)
    {
        *bot = realloc(*bot, (*tam_bot + 2) * sizeof(Cartas));
        (*bot)[*tam_bot] = carta_bot;
        (*bot)[*tam_bot + 1] = carta_jogador;
        *tam_bot = *tam_bot + 2;
        printf("+2 BOT\n");
    }

    if (currentScreen == GAME_EMPATE)
    {
        *jogador = realloc(*jogador, (*tam_jogador + 1) * sizeof(Cartas));
        (*jogador)[*tam_jogador] = carta_jogador;
        *tam_jogador = *tam_jogador + 1;
        printf("+ JOGADOR\n");

        *bot = realloc(*bot, (*tam_bot + 1) * sizeof(Cartas));
        (*bot)[*tam_bot] = carta_bot;
        *tam_bot = *tam_bot + 1;
        printf("+ JOGADOR\n");
    }
}

int main(void)
{
    int conquistas[3] = {0, 0, 0};
    /*
    char *nomeArquivo = "conquistas.csv";
    FILE *fileconq;
    fileconq = fopen(nomeArquivo, "r");
    if (fileconq == NULL)
    {
        printf("Não foi possível abrir o arquivo '%s'. Usando vetor padrão.\n", nomeArquivo);
    }
    else
    {
        // Se o arquivo for aberto com sucesso, lê os valores
        fscanf(fileconq, "%d,%d,%d", &conquistas[0], &conquistas[1], &conquistas[2]);
        fclose(fileconq);
    }
    */

    // Carregamento de cartas
    const int MAX_CARTAS = 32;
    int jogoplay = 0; // marca se o jogo começou ou não
    char filename[] = "cartas.csv";
    int contador = 0;
    char linhas[256];
    int atributo = 0;
    int jogador_valor, bot_valor;
    int ganhador = -1;
    int rodada = 0;
    int vez = PLAYER;
    int continuar;

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
            if (contador == 1)
                continue; // Pula a linha do cabeçalho

            int index = contador - 2;
            if (index >= MAX_CARTAS)
                break;

            char *token = strtok(linhas, ",");
            int coluna = 0;
            while (token)
            {
                coluna++;
                switch (coluna)
                {
                case 2:
                    strncpy(baralho[index].nome, token, sizeof(baralho[index].nome) - 1);
                    break;
                case 3:
                    baralho[index].influencia = atoi(token);
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
                    baralho[index].letra = token[0];
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
        fseek(binario, 0, SEEK_END);
        int tamanhoBinario = ftell(binario);
        int tamanho = tamanhoBinario / sizeof(Cartas);
        baralho = realloc(baralho, sizeof(Cartas) * tamanho);
        fseek(binario, 0, 0);
        fread(baralho, sizeof(Cartas), tamanho, binario);
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
    Texture2D costas = LoadTexture(CAPA_PATH);
    Texture2D empate = LoadTexture(EMPATE_PATH);
    Image mago_i = LoadImage(MAGO_PATH);
    Image nomago_i = LoadImage(NOMAGO_PATH);
    ImageResize(&mago_i, 300, 300);
    ImageResize(&nomago_i, 300, 300);
    Texture2D mago = LoadTextureFromImage(mago_i);
    Texture2D nomago = LoadTextureFromImage(nomago_i);
    Texture2D interogacao = LoadTexture(INTERROGACAO_PATH);
    Texture2D gatook = LoadTexture("resources\\Elementos\\cat\\catok.jpg");
    Texture2D gatotiti = LoadTexture("resources\\Elementos\\cat\\gatotiti.jpg");

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
                currentScreen = ACHIEVEMENTS;
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
                totalCartasBot = MAX_CARTAS/2;
                totalCartasPlayer = MAX_CARTAS/2;
                distribuiCartas(baralho, MAX_CARTAS, Player, Bot);
                jogoplay = 1; // começou um jogo
            }

            // rodada
            if (rodada == 0)
            {
                // gerar cartas atuais
                printf("player: %i\n Bot: %i\n max: %i\n", totalCartasPlayer, totalCartasBot, MAX_CARTAS);
                geraImagemCarta(&cartaPlayer, Player[0]);
                geraImagemCarta(&cartaBot, Bot[0]);
                rodada = 1;
                atributo = 0;
                continuar = 0;
            }

            // Desenha o fundo
            {
                DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
                ClearBackground(RAYWHITE);

                DrawTexture(logopp, 0, 0, WHITE);
                DrawTexture(top, 163, 121, WHITE);

                DrawTexture(cartaPlayer, 203, 349, WHITE);
                DrawTexture(costas, 692, 349, WHITE);
            }

            if (vez == PLAYER)
            {
                // Desenha os botões de atributos
                GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
                {
                    if (GuiButton((Rectangle){39, 845, 253, 57}, "Influência"))
                    {
                        continuar = 1;
                        atributo = INFLUENCIA;
                        jogador_valor = Player[0].influencia;
                        bot_valor = Bot[0].influencia;
                        
                    }
                    if (GuiButton((Rectangle){39 + 1 * (35 + 253), 845, 253, 57}, "Estratégia"))
                    {
                        continuar = 1;
                        atributo = ESTRATEGIA;
                        jogador_valor = Player[0].estrategia;
                        bot_valor = Bot[0].estrategia;
                        
                    }
                    if (GuiButton((Rectangle){39 + 2 * (35 + 253), 845, 253, 57}, "Popularidade"))
                    {
                        continuar = 1;
                        atributo = POPULARIDADE;
                        jogador_valor = Player[0].popularidade;
                        bot_valor = Bot[0].popularidade;
                        
                    }
                    if (GuiButton((Rectangle){39 + 3 * (35 + 253), 845, 253, 57}, "Legado"))
                    {
                        continuar = 1;
                        atributo = LEGADO;
                        jogador_valor = Player[0].legado;
                        bot_valor = Bot[0].legado;
                        
                    }
                } // botoes de atributo
            }
            else
            {
                Cartas porcentagem;
                porcentagem.influencia = (Bot[0].influencia / 5) * 100;
                porcentagem.estrategia = (Bot[0].estrategia / 100) * 100;
                porcentagem.popularidade = (Bot[0].popularidade / 100) * 100;
                porcentagem.legado = (Bot[0].legado / 20) * 100;

                if ((porcentagem.influencia > porcentagem.estrategia) && (porcentagem.influencia > porcentagem.popularidade) && (porcentagem.influencia > porcentagem.legado))
                {
                    atributo = INFLUENCIA;
                    jogador_valor = Player[0].influencia;
                    bot_valor = Bot[0].influencia;
                }
                else if ((porcentagem.estrategia > porcentagem.influencia) && (porcentagem.estrategia > porcentagem.popularidade) && (porcentagem.estrategia > porcentagem.legado))
                {
                    atributo = ESTRATEGIA;
                    jogador_valor = Player[0].estrategia;
                    bot_valor = Bot[0].estrategia;
                }
                else if ((porcentagem.popularidade > porcentagem.estrategia) && (porcentagem.popularidade > porcentagem.popularidade) && (porcentagem.popularidade > porcentagem.legado))
                {
                    atributo = POPULARIDADE;
                    jogador_valor = Player[0].popularidade;
                    bot_valor = Bot[0].popularidade;
                }
                else
                {
                    atributo = LEGADO;
                    jogador_valor = Player[0].legado;
                    bot_valor = Bot[0].legado;
                    
                }
                if (GuiButton((Rectangle){39 + 3 * (35 + 253), 845, 253, 57}, "continue"))
                {
                    continuar = 1;

                }
            }

            if (atributo != 0 && continuar == 1)
            {
                ganhador = DeterminarGanhador(jogador_valor, bot_valor, Player, Bot);
                if (ganhador == PLAYER)
                {
                    if (totalCartasBot - 1 == 0)
                    {
                        currentScreen = PLAYER_WIN;
                    }
                    else
                    {
                        currentScreen = GAME_PLAYER;
                    }
                }
                if (ganhador == BOT)
                {
                    if (totalCartasPlayer - 1 == 0)
                    {
                        currentScreen = BOT_WIN;
                    }
                    else
                    {
                        currentScreen = GAME_BOT;
                    }
                }
                if (ganhador == EMPATE)
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

        if (currentScreen == PROMPTCONQUISTAS)
        {

            DrawRectangle(350, 300, 500, 200, WHITE);
            DrawRectangleLines(350, 300, 500, 200, WHITE);
            DrawTexture(conquista, 438, 340, WHITE);

            if (GuiButton((Rectangle){450, 430, 300, 50}, "continue"))
            {
                currentScreen = previousScreen;
            }
        }

        if (currentScreen == GAME_PLAYER)
        {

            // Desenha o fundo
            {
                GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
                DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
                ClearBackground(RAYWHITE);

                DrawTexture(logopp, 0, 0, WHITE);
                DrawTexture(playerwin, 163, 121, WHITE);

                DrawTexture(cartaPlayer, 203, 349, WHITE);
                DrawTexture(cartaBot, 692, 349, WHITE);
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
                    // ganhou do supertrunfo "ganhou do mago"

                    if (Bot[0].super_trunfo == 1 && !(Player[0].letra == 'A' || Player[0].letra == 'a') && conquistas[0] == 0)
                    {
                        previousScreen = currentScreen;
                        currentScreen = PROMPTCONQUISTAS;
                        conquistas[0] = 1;
                    }
                    AtualizaDeck(&Player, &totalCartasPlayer, &Bot, &totalCartasBot, currentScreen);
                    atributo = 0;
                    rodada = 0;
                    continuar = 0;
                    vez = PLAYER;
                    currentScreen = GAME;
                }
            }

        } // fim do game_player

        if (currentScreen == GAME_BOT)
        {

            // Desenha o fundo
            {
                GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
                DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
                ClearBackground(RAYWHITE);

                DrawTexture(logopp, 0, 0, WHITE);
                DrawTexture(botwin, 163, 121, WHITE);

                DrawTexture(cartaPlayer, 203, 349, WHITE);
                DrawTexture(cartaBot, 692, 349, WHITE);
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
                    // perdeu do supertrunfo "perdeu pro mago"
                    if (Bot[0].super_trunfo == 1 && !(Player[0].letra == 'A' || Player[0].letra == 'a') && conquistas[1] == 0)
                    {
                        previousScreen = currentScreen;
                        currentScreen = PROMPTCONQUISTAS;
                        conquistas[1] = 1;
                    }
                    AtualizaDeck(&Player, &totalCartasPlayer, &Bot, &totalCartasBot, currentScreen);
                    atributo = 0;
                    rodada = 0;
                    continuar = 0;
                    vez = BOT;
                    currentScreen = GAME;
                }
            }

        } // fim do game_bot

        if (currentScreen == GAME_EMPATE)
        {

            // Desenha o fundo
            {
                GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
                DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
                ClearBackground(RAYWHITE);

                DrawTexture(logopp, 0, 0, WHITE);
                DrawTexture(empate, 163, 121, WHITE);

                DrawTexture(cartaPlayer, 203, 349, WHITE);
                DrawTexture(cartaBot, 692, 349, WHITE);
            }

            // botão saida + proxima rodada
            {

                if (GuiButton((Rectangle){1140, 10, 50, 50}, "X"))
                {

                    previousScreen = GAME_EMPATE;
                    currentScreen = CONFIRM_EXIT;
                }

                if (GuiButton((Rectangle){39, 920, 300, 57}, "Próxima Rodada"))
                {
                    if (conquistas[2] == 0)
                    {
                        previousScreen = currentScreen;
                        currentScreen = PROMPTCONQUISTAS;
                        conquistas[2] = 1;
                    }
                    AtualizaDeck(&Player, &totalCartasPlayer, &Bot, &totalCartasBot, currentScreen);
                    atributo = 0;
                    rodada = 0;
                    continuar = 0;
                    currentScreen = GAME;
                }
            }

        } // fim do game_EMPATE

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

        if (currentScreen == ACHIEVEMENTS)
        {

            GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
            DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
            ClearBackground(RAYWHITE);

            if (conquistas[0] == 0)
            {
                DrawTexture(nomago, 86, 267, WHITE);
                DrawTexture(interogacao, 111, 588, WHITE);
            }
            else
            {
                DrawTexture(mago, 86, 267, WHITE);
            }
            if (conquistas[1] == 0)
            {
                DrawTexture(nomago, 448, 267, WHITE);
            }
            else
            {
                DrawTexture(mago, 448, 267, WHITE);
            }
            if (conquistas[2] == 0)
            {
                DrawTexture(nomago, 810, 267, WHITE);
            }
            else
            {
                DrawTexture(mago, 810, 267, WHITE);
            }
            // saida
            if (GuiButton((Rectangle){1140, 10, 50, 50}, "X"))
            {
                previousScreen = GAME_PLAYER;
                currentScreen = CONFIRM_EXIT;
            }
        }
        if (currentScreen == BOT_WIN)
        {
            // Desenha o fundo
            {
                GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
                DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
                ClearBackground(RAYWHITE);
            }

            if (GuiButton((Rectangle){1140, 10, 50, 50}, "X"))
            {

                previousScreen = GAME_BOT;
                currentScreen = CONFIRM_EXIT;
            }
        }
        if (currentScreen == PLAYER_WIN)
        {
            // Desenha o fundo
            {

                GuiSetStyle(DEFAULT, TEXT_SIZE, 75);
                DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
                ClearBackground(RAYWHITE);
            }

            if (GuiButton((Rectangle){100, 100, 1000, 150}, "Você ganhou! Parabens"))
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
            DrawTexture(gatook, 300, 400, WHITE);
        }

        if (currentScreen == BOT_WIN)
        {
            // Desenha o fundo
            {

                GuiSetStyle(DEFAULT, TEXT_SIZE, 75);
                DrawTexture(background, BACKGROUND_X, BACKGROUND_Y, WHITE);
                ClearBackground(RAYWHITE);
                DrawTexture(gatotiti, 300, 400, WHITE);
            }

            if (GuiButton((Rectangle){100, 100, 1000, 150}, "Você perdeu! :((("))
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
        }

        EndDrawing();
    } // whille (WindowShouldClose)
/*
    fileconq = fopen("conquistas.csv", "w");
    if (fileconq == NULL)
    {
        printf("Erro ao criar o arquivo 'conquistas.csv'.\n");
        return 1;
    }
*/
    // Unload textures
    UnloadTexture(logo);
    UnloadTexture(subtitulo);
    UnloadTexture(background);

    // Fecha a janela
    CloseWindow();

    return 0;
}
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

// Enum para determinar quem joga no momento
typedef enum
{
    JOGADOR,
    MAQUINA
} Rodada;

// Carregamento de imagens das cartas
void geraImagemCarta(Texture2D *carta, Cartas cartaInfo)
{
    char filePath[] = "resources\\cartas\\";

    int i = cartaInfo.numero;
    char b[2];
    b[0] = cartaInfo.letra;
    b[1] = '\0';
    char buffer[33];

    printf("file: %s\n", filePath);

    strcat(filePath, b);

    printf("file: %s\n", filePath);

    itoa(i, buffer, 10);

    strcat(filePath, buffer);

    printf("file: %s\n", filePath);

    // Carrega a imagem
    Image carta_i = LoadImage(filePath);

    // Verifica se a imagem foi carregada corretamente
    if (carta_i.data == NULL)
    {
        carta_i = LoadImage("resources\\cartas\\cat.png");
    }

    // Redimensiona a imagem
    ImageResize(&carta_i, 300, 450);

    // Carrega a textura a partir da imagem
    *carta = LoadTextureFromImage(carta_i);

    // Libera a imagem da memória
    UnloadImage(carta_i);
}
// exibir atributos
void DrawCardAttributes(Cartas *carta, int posX, int posY)
{
    DrawText("Atributos da Carta:", posX, posY, 20, BLACK);
    DrawText(TextFormat("Nome: %s", carta->nome), posX, posY + 30, 20, BLACK);
    DrawText(TextFormat("Influência: %d", carta->influencia), posX, posY + 60, 20, BLACK);
    DrawText(TextFormat("Estratégia: %d", carta->estrategia), posX, posY + 90, 20, BLACK);
    DrawText(TextFormat("Popularidade: %d", carta->popularidade), posX, posY + 120, 20, BLACK);
    DrawText(TextFormat("Legado: %d", carta->legado), posX, posY + 150, 20, BLACK);
    DrawText(TextFormat("Código: %c-%d", carta->letra, carta->numero), posX, posY + 180, 20, BLACK);
}

int main()
{
    // Inicialização da janela
    const int SCREEN_WIDTH = 1200;
    const int SCREEN_HEIGHT = 1000;
    const int FPS = 60;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SuperTrunfo");
    SetTargetFPS(FPS);

    // Inicialização das variáveis de controle de tela
    Screen currentScreen = MAIN_MENU;
    Screen previousScreen = MAIN_MENU;
    GuiSetStyle(DEFAULT, TEXT_SIZE, 50);

    // Carregamento de cartas
    const int MAX_CARTAS = 32;
    int jogoplay = 1;
    char filename[] = "cartas.csv";
    int contador = 0;
    char linhas[256];
    int atributo = 0;
    int jogador_valor, bot_valor;
    int ganhador = -1;

    // Alocação de memória para as cartas do jogador e da máquina
    Cartas *lista = (Cartas *)malloc(sizeof(Cartas) * MAX_CARTAS);
    int QNT_CARTAS_PLAYER = MAX_CARTAS / 2;
    int QNT_CARTAS_MAQUINA = MAX_CARTAS / 2;
    Cartas *Player = (Cartas *)malloc(sizeof(Cartas) * (QNT_CARTAS_PLAYER));
    Cartas *Maquina = (Cartas *)malloc(sizeof(Cartas) * (QNT_CARTAS_MAQUINA));
    if (lista == NULL)
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
                    strncpy(lista[index].nome, token, sizeof(lista[index].nome) - 1);
                    break;
                case 3:
                    lista[index].influencia = atoi(token);
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
                    lista[index].letra = token[0];
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
    else
    {
        printf("Bem-vindo de volta!\n");
        fseek(binario, 0, SEEK_END);
        int tamanhoBinario = ftell(binario);
        int tamanho = tamanhoBinario / sizeof(Cartas);
        lista = realloc(lista, sizeof(Cartas) * tamanho);
        fseek(binario, 0, 0);
        fread(lista, sizeof(Cartas), tamanho, binario);
        fclose(binario);
    }

    fclose(file);

    Texture2D cartaPlayer;
    geraImagemCarta(&cartaPlayer, Player[0]);
    Texture2D cartaMaquina;
    geraImagemCarta(&cartaMaquina, Maquina[0]);

    // Dimensões e posições
    const int CARD_WIDTH = 200;
    const int CARD_HEIGHT = 300;
    const int BUTTON_WIDTH = 200;
    const int BUTTON_HEIGHT = 60;
    const int BUTTON_SPACING = 20;
    const int TOTAL_BUTTONS_WIDTH = (BUTTON_WIDTH + BUTTON_SPACING) * 4 - BUTTON_SPACING;
    const int BUTTON_X = (SCREEN_WIDTH - TOTAL_BUTTONS_WIDTH) / 2;
    const int BUTTON_Y = SCREEN_HEIGHT / 2 + CARD_HEIGHT / 2 + 20;

    const int CARD_Y = SCREEN_HEIGHT / 2 - CARD_HEIGHT / 2 - 50;
    const int PLAYER_CARD_X = SCREEN_WIDTH / 2 - CARD_WIDTH - 20;
    const int MACHINE_CARD_X = PLAYER_CARD_X + CARD_WIDTH + 40;
    const int TEXT_X = 20;
    const int TEXT_Y = CARD_Y;

    // Loop principal do jogo
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawFPS(10, 10);
        const char *title = "SuperTrunfo";
        int fontSizeTitle = 100;
        int titleWidth = MeasureText(title, fontSizeTitle);
        int titleX = (SCREEN_WIDTH - titleWidth) / 2;
        int titleY = SCREEN_HEIGHT / 4;

        if (currentScreen == MAIN_MENU)
        {
            DrawText(title, titleX, titleY, fontSizeTitle, BLACK);

            // Definindo a largura e altura dos botões
            int buttonWidth = 300;
            int buttonHeight = 150;

            // Calculando a posição do botão "Jogar"
            Rectangle buttonBounds = {(SCREEN_WIDTH - buttonWidth) / 2, titleY + fontSizeTitle + 50, buttonWidth, buttonHeight};
            if (GuiButton(buttonBounds, "Jogar"))
            {
                currentScreen = GAME;
            }

            // Calculando a posição do botão "Conquistas"
            buttonBounds.y += buttonHeight + 20; // Espaço entre os botões
            if (GuiButton(buttonBounds, "Conquistas"))
            {
                currentScreen = ACHIEVEMENTS;
            }
        }
        else if (currentScreen == GAME)
        {
            if (jogoplay == 1)
            {
                distribuiCartas(lista, MAX_CARTAS, Player, Maquina);
                jogoplay = 0;
            }
            // desenhos cartas jogador
            DrawText("Sua Carta", PLAYER_CARD_X + CARD_WIDTH / 2 - MeasureText("Sua Carta", 20) / 2, CARD_Y - 30, 20, BLACK);
            DrawText("Carta Máquina", MACHINE_CARD_X + CARD_WIDTH / 2 - MeasureText("Carta Máquina", 20) / 2, CARD_Y - 30, 20, BLACK);

            DrawTexture(cartaPlayer, PLAYER_CARD_X, CARD_Y, WHITE);
            DrawRectangleLines(PLAYER_CARD_X, CARD_Y, CARD_WIDTH, CARD_HEIGHT, DARKGRAY);
            DrawTexture(cartaMaquina, MACHINE_CARD_X, CARD_Y, WHITE);
            DrawRectangleLines(MACHINE_CARD_X, CARD_Y, CARD_WIDTH, CARD_HEIGHT, DARKGRAY);
            // atributos jogador
            DrawCardAttributes(&Player[0], TEXT_X, TEXT_Y);

            if (GuiButton((Rectangle){BUTTON_X, BUTTON_Y, BUTTON_WIDTH + 20, BUTTON_HEIGHT}, "Influência"))
            {
                atributo = 1;
            }
            if (GuiButton((Rectangle){BUTTON_X + (BUTTON_WIDTH + BUTTON_SPACING) * 1, BUTTON_Y, BUTTON_WIDTH + 20, BUTTON_HEIGHT}, "Estratégia"))
            {
                atributo = 2;
            }
            if (GuiButton((Rectangle){BUTTON_X + (BUTTON_WIDTH + BUTTON_SPACING) * 2, BUTTON_Y, BUTTON_WIDTH +20, BUTTON_HEIGHT}, "Popularidade"))
            {
                atributo = 3;
            }
            if (GuiButton((Rectangle){BUTTON_X + (BUTTON_WIDTH + BUTTON_SPACING) * 3, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT}, "Legado"))
            {
                atributo = 4;
            }

            if (atributo != 0)
            {
                switch (atributo)
                {
                case 1:
                    jogador_valor = Player[0].influencia;
                    bot_valor = Maquina[0].influencia;
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
                    currentScreen = (Maquina[0].letra == 'A' || Maquina[0].letra == 'a') ? GAME_Maquina : GAME_Player;
                }
                else if (Maquina[0].super_trunfo == 1)
                {
                    currentScreen = (Player[0].letra == 'A' || Player[0].letra == 'a') ? GAME_Player : GAME_Maquina;
                }
                else
                {
                    currentScreen = (jogador_valor > bot_valor) ? GAME_Player : GAME_Maquina;
                }
            }

            if (GuiButton((Rectangle){1050, 20, 150, 60}, "Voltar"))
            {
                previousScreen = GAME;
                currentScreen = CONFIRM_EXIT;
            }
        }

        if (currentScreen == GAME_Player)
        {
            DrawCardAttributes(&Player[0], TEXT_X, TEXT_Y);

            DrawCardAttributes(&Maquina[0], MACHINE_CARD_X + CARD_WIDTH + 20, TEXT_Y);

            const char text[] = "Você ganhou a rodada!";
            int fontSize = 40;
            int textWidth = MeasureText(text, fontSize);
            int posX = (1200 - textWidth) / 2;
            DrawText(text, posX, 20, fontSize, BLACK);

            if (GuiButton((Rectangle){BUTTON_X, BUTTON_Y + BUTTON_HEIGHT, BUTTON_WIDTH * 2, BUTTON_HEIGHT}, "Próxima Rodada"))
            {
                Player = realloc(Player, (QNT_CARTAS_PLAYER + 1) * sizeof(Cartas));
                Player[QNT_CARTAS_PLAYER] = Maquina[0];
                QNT_CARTAS_PLAYER++;
                for (int i = 0; i < QNT_CARTAS_MAQUINA - 1; i++)
                {
                    Maquina[i] = Maquina[i + 1];
                }
                QNT_CARTAS_MAQUINA--;
                Maquina = realloc(Maquina, QNT_CARTAS_MAQUINA * sizeof(Cartas));
                atributo = 0;
                currentScreen = GAME;
            }

            if (GuiButton((Rectangle){1050, 20, 150, 60}, "Voltar"))
            {
                previousScreen = GAME_Player;
                currentScreen = CONFIRM_EXIT;
            }
        }

        if (currentScreen == GAME_Maquina)
        {
            DrawCardAttributes(&Player[0], TEXT_X, TEXT_Y);

            DrawCardAttributes(&Maquina[0], MACHINE_CARD_X + CARD_WIDTH + 20, TEXT_Y);

            const char text[] = "Máquina ganhou a rodada!";
            int fontSize = 40;
            int textWidth = MeasureText(text, fontSize);
            int posX = (1200 - textWidth) / 2;
            DrawText(text, posX, 20, fontSize, BLACK);

            if (GuiButton((Rectangle){BUTTON_X, BUTTON_Y + BUTTON_HEIGHT, BUTTON_WIDTH * 2, BUTTON_HEIGHT}, "Próxima Rodada"))
            {
                Maquina = realloc(Maquina, (QNT_CARTAS_MAQUINA + 1) * sizeof(Cartas));
                Maquina[QNT_CARTAS_MAQUINA] = Player[0];
                QNT_CARTAS_MAQUINA++;
                for (int i = 0; i < QNT_CARTAS_PLAYER - 1; i++)
                {
                    Player[i] = Player[i + 1];
                }
                QNT_CARTAS_PLAYER--;
                Player = realloc(Player, QNT_CARTAS_PLAYER * sizeof(Cartas));
                atributo = 0;
                currentScreen = GAME;
            }

            if (GuiButton((Rectangle){1050, 20, 150, 60}, "Voltar"))
            {
                previousScreen = GAME_Maquina;
                currentScreen = CONFIRM_EXIT;
            }
        }

        if (currentScreen == CONFIRM_EXIT)
        {
            DrawRectangle(300, 300, 600, 200, LIGHTGRAY);
            DrawRectangleLines(300, 300, 600, 200, DARKGRAY);
            DrawText("Tem certeza que deseja voltar?", 400, 350, 20, BLACK);

            if (GuiButton((Rectangle){400, 450, 100, 50}, "Sim"))
            {
                currentScreen = MAIN_MENU;
                jogoplay = 1;
            }
            if (GuiButton((Rectangle){700, 450, 100, 50}, "Não"))
            {
                currentScreen = previousScreen;
            }
        }

        EndDrawing();
    }

    UnloadTexture(cartaPlayer);
    UnloadTexture(cartaMaquina);
    free(Player);
    free(Maquina);
    free(lista);
    CloseWindow();

    return 0;
}
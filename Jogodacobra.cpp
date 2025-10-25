#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>


//Tamanho do mapa
#define LARGURA 30
#define ALTURA 15
#define TAM_MAX 100



typedef struct {
    int x;
    int y;
} Ponto;



typedef struct {
    Ponto corpo[TAM_MAX];
    int tamanho;
    char direcao;
} Cobra;

Ponto comida;
Cobra cobra;
int pontuacao;
int fimJogo;

void iniciarJogo();
void desenhar();
void gerarComida();
void moverCobra();
int verificarColisao(int x, int y);
int cobraOcupa(int x, int y);

void iniciarJogo() {
    cobra.tamanho = 3;
    cobra.direcao = 'd';
    int cx = LARGURA / 2;
    int cy = ALTURA / 2;
    cobra.corpo[0].x = cx; cobra.corpo[0].y = cy;
    cobra.corpo[1].x = cx - 1; cobra.corpo[1].y = cy;
    cobra.corpo[2].x = cx - 2; cobra.corpo[2].y = cy;
    srand(time(NULL));
    gerarComida();
    pontuacao = 0;
    fimJogo = 0;
}

void desenhar() {
    system("cls");
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < LARGURA; j++) {
            int impresso = 0;
            if (i == 0 || i == ALTURA - 1 || j == 0 || j == LARGURA - 1) {
                printf("#");
                impresso = 1;
            } else if (j == comida.x && i == comida.y) {
                printf("*");
                impresso = 1;
            } else {
                for (int k = 0; k < cobra.tamanho; k++) {
                    if (cobra.corpo[k].x == j && cobra.corpo[k].y == i) {
                        printf("O");
                        impresso = 1;
                        break;
                    }
                }
            }
            if (!impresso) printf(" ");
        }
        printf("\n");
    }
    printf("Pontuação: %d\n", pontuacao);
    printf("Controlo: W A S D  |  ESC para sair\n");
}

void gerarComida() {
    int fx, fy;
    do {
        fx = (rand() % (LARGURA - 2)) + 1;
        fy = (rand() % (ALTURA - 2)) + 1;
    } while (cobraOcupa(fx, fy));
    comida.x = fx;
    comida.y = fy;
}

int cobraOcupa(int x, int y) {
    for (int i = 0; i < cobra.tamanho; i++) {
        if (cobra.corpo[i].x == x && cobra.corpo[i].y == y) return 1;
    }
    return 0;
}

int verificarColisao(int x, int y) {
    if (x == 0 || x == LARGURA - 1 || y == 0 || y == ALTURA - 1) return 1;
    for (int i = 0; i < cobra.tamanho; i++) {
        if (cobra.corpo[i].x == x && cobra.corpo[i].y == y) return 1;
    }
    return 0;
}

void moverCobra() {
    Ponto novaCabeca = cobra.corpo[0];
    if (cobra.direcao == 'w') novaCabeca.y -= 1;
    else if (cobra.direcao == 's') novaCabeca.y += 1;
    else if (cobra.direcao == 'a') novaCabeca.x -= 1;
    else if (cobra.direcao == 'd') novaCabeca.x += 1;

    if (verificarColisao(novaCabeca.x, novaCabeca.y)) {
        fimJogo = 1;
        return;
    }

    int comeu = (novaCabeca.x == comida.x && novaCabeca.y == comida.y);

    for (int i = cobra.tamanho - 1; i > 0; i--) {
        cobra.corpo[i] = cobra.corpo[i - 1];
    }
    cobra.corpo[0] = novaCabeca;

    if (comeu) {
        if (cobra.tamanho < TAM_MAX - 1) {
            cobra.tamanho++;
            cobra.corpo[cobra.tamanho - 1] = cobra.corpo[cobra.tamanho - 2];
        }
        pontuacao += 10;
        gerarComida();
    }
}

int main() {
    iniciarJogo();
    while (!fimJogo) {
        desenhar();
        if (_kbhit()) {
            int c = _getch();
            if (c == 27) {
                fimJogo = 1;
                break;
            }
            if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
            if (c == 'w' && cobra.direcao != 's') cobra.direcao = 'w';
            if (c == 's' && cobra.direcao != 'w') cobra.direcao = 's';
            if (c == 'a' && cobra.direcao != 'd') cobra.direcao = 'a';
            if (c == 'd' && cobra.direcao != 'a') cobra.direcao = 'd';
        }
        moverCobra();
        Sleep(120);
    }
    system("cls");
    printf("Fim do jogo! Pontuação final: %d\n", pontuacao);
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // Use "unistd.h" e "ncurses.h" no Linux
#include <windows.h> // Para a função Sleep no Windows

#define WIDTH 20
#define HEIGHT 20

int gameOver;
int paused;
int x, y, fruitX, fruitY, score; // Posição cobra/player/score player 1
int x2, y2, fruitX2, fruitY2, score2; // Posição cobra/player/score player 2
int tailX[100], tailY[100]; //  cauda player 1
int tailX2[100], tailY2[100]; // cauda player 2
int tailLength; // Tamanho cauda player 1
int tailLength2; // Tamanho cauda player 2
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum Direction dir; // direção player 1
enum Direction dir2; // direção player 2
HANDLE console;
int currentColor;
int speed;

void setupmulti() {
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    gameOver = 0;
    paused = 0;
    dir = STOP; // Direção Jogador 1
    dir2 = STOP; // Direção Jogador 2
    x = WIDTH / 3; // Jogador 1 começa à esquerda
    y = HEIGHT / 2;
    x2 = 2 * WIDTH / 3; // Jogador 2 começa à direita
    y2 = HEIGHT / 2; 
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
    score2 = 0;
    tailLength = 0; // Cauda inicial Player 1
    tailLength2 = 0; // Cauda inicial Player 2
    currentColor = 7; // Cor padrão do console é 7 (branco)
    SetConsoleTextAttribute(console, currentColor);
}

void changeColor() {
    int newColor = rand() % 15+1;
    currentColor = newColor;
    SetConsoleTextAttribute(console, currentColor);
}

void draw() {
    system("cls"); // Limpa o console (use "clear" no Linux)
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) printf("#");
            if (i == y && j == x) {
                SetConsoleTextAttribute(console, 10); // Cor player 1 - verde
                printf("O"); // Cabeca da cobra
                SetConsoleTextAttribute(console, currentColor);
            } else if (i == y2 && j == x2) {
                SetConsoleTextAttribute(console, 12); // Cobra 2 vermelha
                printf("X");
                SetConsoleTextAttribute(console, currentColor);
            } else if (i == fruitY && j == fruitX) {
                printf("F"); // Fruta
          } else {
                int isTail = 0;
                for (int k = 0; k < tailLength; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o");
                        isTail = 1;
                    }
                }
                for (int k = 0; k < tailLength2; k++) {
                    if (tailX2[k] == j && tailY2[k] == i) {
                        printf("x");
                        isTail = 1;
                    }
                }
                if (!isTail) printf(" ");
            }
            if (j == WIDTH - 1) printf("#");
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    printf("Player 1 Score: %d | Player 2 Score %d\n ", score, score2);
    if (paused) {
        printf("\n[JOGO PAUSADO] Pressione P novamente para continuar\n");
    }
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
            // Controle Player 1
            case 'a': if (!paused) dir = LEFT; break;
            case 'd': if (!paused) dir = RIGHT; break;
            case 'w': if (!paused) dir = UP; break;
            case 's': if (!paused) dir = DOWN; break;
                    
            // controle Player 2
            case 75: if (!paused) dir2 = LEFT; break; // Seta esquerda
            case 77: if (!paused) dir2 = RIGHT; break; // Seta direita
            case 72: if (!paused) dir2 = UP; break; // Seta cima
            case 80: if (!paused) dir2 = DOWN; break; // Seta baixo

            // Pausar e Sair
            case 'x': gameOver = 1; break;
            case 'p': paused = !paused; break;

        }
    }
}

void playFruitSound() {
    Beep(1000, 200); // Frequência de 1000 Hz por 200 ms
}

void playGameOverSound() {
    Beep(400, 300);
    Beep(300, 300);
    Beep(200, 400);
}

void logic() {
     if (paused) return;

     // Aqui atualiza a cauda do player 1
    int prevX = tailX[0], prevY = tailY[0], prev2X, prev2Y;
    tailX[0] = x; tailY[0] = y;
    for (int i = 1; i < tailLength; i++) {
        prev2X = tailX[i]; 
        prev2Y = tailY[i];
        tailX[i] = prevX; tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Aqui atualiza a cauda do player 2
    prevX = tailX2[0];
    prevY = tailY2[0];
    tailX2[0] = x2;
    tailY2[0] = y2;
    for (int i = 1; i < tailLength2; i++) {
        prev2X = tailX2[i];
        prev2Y = tailY2[i];
        tailX2[i] = prevX;
        tailY2[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Aqui movimenta o player 1
    switch (dir) {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
        default: break;
    }


    // Aqui movimenta o player 2
  switch (dir2) {
        case LEFT: x2--; break;
        case RIGHT: x2++; break;
        case UP: y2--; break;
        case DOWN: y2++; break;
        default: break;
    }

    // Verifica se bateu na parede (ambos players)
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0) gameOver = 1;
    if (x2 >= WIDTH || x2 <0 || y2 >= HEIGHT || y2 < 0) gameOver = 1;

    // Verifica se bateu no próprio corpo
    for (int i = 0; i < tailLength; i++) {
        if (tailX[i] == x && tailY[i] == y) gameOver = 1;
    }
    for (int i = 0; i < tailLength2; i++) {
        if (tailX2[i] == x2 && tailY2[i] == y2) gameOver = 1;
    }

    // Colisão entre os próprios players

    if (x == x2 && y ==y2) gameOver = 1;

    // Verifica se comeu a fruta
    if (x == fruitX && y == fruitY) {
        score += 50;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        tailLength++;
        playFruitSound();
        changeColor();
    }
    if (x2 == fruitX && y2 == fruitY) {
        score2 += 50;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        tailLength2++;
        playFruitSound();
        changeColor();
    }

    int speed = 100 - (score/ 10);
    if (speed <30) speed = 30;
    Sleep(speed);

}

//Estrutura para armazenar o highscore
typedef struct {
    char name[20];
    int score;
} HighScore;

HighScore highScore = {"", 0};

//Função para ler o HighScore do arquivo
void loadHighScore() {
    FILE *file = fopen("highscore.txt","r");
    if (file != NULL) {
        fscanf(file, "%s %d", highScore.name, &highScore.score);
    }
}

//Função para salvar o HighScore do arquivo
void saveHighScore() {
    FILE *file = fopen("highscore.txt", "w");
    if (file!= NULL) {
        fprintf(file, "%s %d", highScore.name, highScore.score);
        fclose(file);
    }
}

//Verifica e atualiza o HighScore
void checkHighScore() {
    if (score > highScore.score) {
        printf("\nNovo Highscore Negueba! Digite seu nome: ");
        scanf("%s", highScore.name);
        highScore.score = score;
        saveHighScore();
    }
}

void showMenu() {
    system("cls"); // Limpa o console (use "clear" no Linux)
    printf("#######################################\n");
    printf("#            JOGO DA COBRINHA         #\n");
    printf("#######################################\n");
    printf("#                                     #\n");
    printf("#   1. Iniciar Jogo                   #\n");
    printf("#   2. Instrucoes                     #\n");
    printf("#   3. Sair                           #\n");
    printf("#                                     #\n");
    printf("#######################################\n");
    printf("Highscore: %s - %d pontos \n", highScore.name, highScore.score);
    printf("Escolha uma opcao: ");
}

void showsubMenu() {
    system("cls"); // Limpa o console (use "clear" no Linux)
    printf("#######################################\n");
    printf("#            JOGO DA COBRINHA         #\n");
    printf("#######################################\n");
    printf("#                                     #\n");
    printf("#   1. SinglePlayer                   #\n");
    printf("#   2. Multiplayer                    #\n");
    printf("#   3. Sair                           #\n");
    printf("#                                     #\n");
    printf("#######################################\n");
    printf("Escolha uma opcao: ");
   
}

void showInstructions() {
    system("cls");
    printf("############ INSTRUCOES ############\n");
    printf("- Use as teclas W, A, S, D para mover a cobra.\n");
    printf("- O objetivo e comer as frutas (F).\n");
    printf("- Evite bater nas paredes ou no proprio corpo.\n");
    printf("- Pressione 'X' para sair do jogo.\n");
    printf("####################################\n");
    printf("\nPressione qualquer tecla para voltar ao menu...");
    _getch();
}

int main() {
    loadHighScore(); // Chama o highscore quando abre o programa
    int choice;
    do {
        showMenu();
        choice = _getch() - '0'; // Captura a entrada e converte para inteiro
        switch (choice) {
            case 1:  // iniciar o jogo
                showsubMenu(); // Exibe o submenu
                int subChoice = _getch() - '0'; // Captua a escolha do submenu
                switch (subChoice) {
                    case 1: // SinglePlayer // Inicia singleplayer
                        setupmulti();
                while (!gameOver) {
                    draw();
                    input();
                    logic();
                    Sleep(80); // Controla a velocidade do jogo
                }
                checkHighScore();
                playGameOverSound(); // Se perder toca música de gameover
                printf("\nGame Over Player 1! Sua pontuacao foi: %d\n", score);
                printf("\nGame Over Player 2! Sua pontuacao foi: %d\n", score2);
                printf("Pressione qualquer tecla para voltar ao menu...");
                _getch();
                break; // Adiciona break para evitar fallthrough
                break;
                
                    case 2: // Multiplayer
                        setupmulti();
                while (!gameOver) {
                    draw();
                    input();
                    logic();
                    Sleep(80); // Controla a velocidade do jogo
                }
                checkHighScore();
                playGameOverSound(); // Se perder toca música de gameover
                printf("\nGame Over Player 1! Sua pontuacao foi: %d\n", score);
                printf("\nGame Over Player 2! Sua pontuacao foi: %d\n", score2);
                printf("Pressione qualquer tecla para voltar ao menu...");
                _getch();
                break; // Adiciona break para evitar fallthrough

                    case 3: // Voltar ao Menu Principal
                        break; // Apenas sai do submenu e volta ao menu principal
                    default:
                        printf("Opcao invalida! Retornando ao menu principal..\n");
                        break;
                        break;
        } // Aqui encerra a função subchoice do submenu dentro da choice do menu principal
               break; // Fim do case 1 (Iniciar o jogo)         
        case 2:
            showInstructions();
            break;
        case 3:
            printf("\nSaindo do jogo... Obrigado por jogar!\n");
            break;
            
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                Sleep(1000);
                break;
           
        }
    
    
    } while (choice != 3);

    return 0;
}

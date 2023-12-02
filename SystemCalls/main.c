#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define BOARD_SIZE 8

// Estrutura do estado do jogo
typedef struct {
  int board[BOARD_SIZE][BOARD_SIZE];
  int player_boards[2][BOARD_SIZE * BOARD_SIZE];
  int current_player;
  bool game_over;
} GameState;

int semid;
GameState *game_state;

void initialize_game() {
  game_state->board[0][0] = 1;
  game_state->board[BOARD_SIZE - 1][BOARD_SIZE - 1] = 2;
  game_state->current_player = 0;
  game_state->game_over = false;
}

// Verifica se o tabuleiro está cheio
bool is_board_full() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (game_state->board[i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}

// Verifica o vencedor
void check_winner() {
  if (is_board_full()) {
    int count_player_1 = 0;
    int count_player_2 = 0;
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
      if (game_state->player_boards[0][i] == 1) {
        count_player_1++;
      }
      if (game_state->player_boards[1][i] == 2) {
        count_player_2++;
      }
    }
    if (count_player_1 > count_player_2) {
      printf("Parabens, Jogador 1! Voce e o campeao!\n");
    } else if (count_player_2 > count_player_1) {
      printf("Parabens, Jogador 2! Voce e o campeao!\n");
    } else {
      printf("O jogo terminou em empate!\n");
    }
    game_state->game_over = true;
  }
}

// Imprime o tabuleiro
void print_board() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (game_state->board[i][j] == 1) {
        printf("V ");
      } else if (game_state->board[i][j] == 2) {
        printf("P ");
      } else {
        printf("_ ");
      }
    }
    printf("\n");
  }
  printf("\n");
}

// Verifica se o movimento é válido
bool is_valid_move(int row, int col) {
  if (row >= BOARD_SIZE || col >= BOARD_SIZE) {
    return false;
  }
  if (game_state->board[row][col] != 0) {
    return false;
  }

  // Verifique se a posição é adjacente a uma peça do jogador atual
  int adjacent_positions[8][2] = {{0, 1}, {1, 0},  {0, -1}, {-1, 0},
                                  {1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
  for (int i = 0; i < 8; i++) {
    int dx = adjacent_positions[i][0];
    int dy = adjacent_positions[i][1];
    int new_row = row + dx;
    int new_col = col + dy;

    if (new_row >= 0 && new_row < BOARD_SIZE && new_col >= 0 &&
        new_col < BOARD_SIZE) {
      if (game_state->board[new_row][new_col] ==
          game_state->current_player + 1) {
        return true;
      }
    }
  }

  return false;
}

// Realiza o movimento
void make_move(int row, int col) {
  game_state->board[row][col] = game_state->current_player + 1;
  game_state->current_player = 1 - game_state->current_player;
}

// Processo do jogador
void player_process(int player) {
    while (!game_state->game_over) {
        if (game_state->current_player == player) {
            print_board();
            check_winner();
            int row, col;
            bool valid_move_exists = false;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (is_valid_move(i, j)) {
                        valid_move_exists = true;
                        break;
                    }
                }
                if (valid_move_exists) {
                    break;
                }
            }
            if (!valid_move_exists && is_board_full) {
                printf("Jogador %d nao tem movimentos validos. Jogador %d e o vencedor!\n", player + 1, 2 - player);
                game_state->game_over = true;
                break;
            }
            printf("Jogador %d, digite a linha (0-%d): ", player + 1, BOARD_SIZE - 1);
            scanf("%d", &row);
            printf("Jogador %d, digite a coluna (0-%d): ", player + 1, BOARD_SIZE - 1);
            scanf("%d", &col);
            if (is_valid_move(row, col)) {
                make_move(row, col);
            } else {
                printf("Movimento invalido. Tente novamente.\n");
            }
        }
    }
    printf("Processo Jogador %d concluido\n", player + 1);
}


int main() {
  printf("Programa iniciado\n");

  key_t key = ftok("shmfile", 65);

  int shmid = shmget(key, sizeof(GameState), 0666 | IPC_CREAT);
  game_state = (GameState *)shmat(shmid, (void *)0, 0);

  initialize_game();

  pid_t pid = fork();
  if (pid == 0) {
    player_process(0);
  } else {
    player_process(1);
  }

  shmdt(game_state);
  shmctl(shmid, IPC_RMID, NULL);

  printf("Programa concluido\n");

  return 0;
}

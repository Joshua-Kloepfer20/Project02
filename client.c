#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>

#define WHITEPAWN 1
#define WHITEKING 2
#define WHITEQUEEN 3
#define WHITEBISHOP 4
#define WHITEKNIGHT 5
#define WHITEROOK 6
#define BLACKPAWN 7
#define BLACKKING 8
#define BLACKQUEEN 9
#define BLACKBISHOP 10
#define BLACKKNIGHT 11
#define BLACKROOK 12

struct user {char username[50]; char password[50]; int rating; int logged; };
struct move {int rcur; int ccur; int r; int c; int check; };
struct piece {SDL_Surface * image; int type; int moves; /*for pawns*/ void (*movecheck)(struct square **, int, int, int **); };
struct square {SDL_Rect rect; struct piece * cpiece; };

int client_handshake() {
  struct addrinfo * hints;
  struct addrinfo * results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, "1119", hints, &results);
  int sd = socket(results->ai_family, results->ai_socktype, 0);
  connect(sd, results->ai_addr, results->ai_addrlen);
  printf("connected\n");
  return sd;
}

void RookMoveCheck(struct square gboard[8][8], int r, int c, int board[8][8]) {
    int rcheck, ccheck;
    memset(board, 0, sizeof(int) * 64);
    if (r < 7) {
        rcheck = r + 1;
        ccheck = c;
        while (rcheck < 8) {
          if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEROOK) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKROOK))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEROOK) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKROOK)) {
            break;
          }
          board[rcheck][ccheck] = 1;
          rcheck++;
        }
    }
    if (r > 0) {
        rcheck = r - 1;
        ccheck = c;
        while (rcheck >= 0) {
          if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEROOK) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKROOK))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEROOK) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKROOK)) {
            break;
          }
          board[rcheck][ccheck] = 1;
          rcheck--;
        }
    }
    if (c < 7) {
        rcheck = r;
        ccheck = c + 1;
        while (ccheck < 8) {
          if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEROOK) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKROOK))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEROOK) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKROOK)) {
            break;
          }
          board[rcheck][ccheck] = 1;
          ccheck++;
        }
    }
    if (c > 0) {
        rcheck = r;
        ccheck = c - 1;
        while (ccheck >= 0) {
          if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEROOK) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKROOK))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEROOK) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKROOK)) {
            break;
          }
          board[rcheck][ccheck] = 1;
          ccheck--;
        }
    }
}

void KnightMoveCheck(struct square gboard[8][8], int r, int c, int board[8][8]) {\
    int rcheck, ccheck;
    memset(board, 0, sizeof(int) * 64);
    rcheck = r + 2;
    ccheck = c + 1;
    if (rcheck < 8 && ccheck < 8 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEKNIGHT) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKKNIGHT))) {
        board[rcheck][ccheck] = 1;
    }
    rcheck = r + 2;
    ccheck = c - 1;
    if (rcheck < 8 && ccheck >= 0 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEKNIGHT) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKKNIGHT))) {
        board[rcheck][ccheck] = 1;
    }
    rcheck = r - 2;
    ccheck = c + 1;
    if (rcheck >= 0 && ccheck < 8 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEKNIGHT) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKKNIGHT))) {
        board[rcheck][ccheck] = 1;
    }
    rcheck = r - 2;
    ccheck = c - 1;
    if (rcheck >= 0 && ccheck >= 0 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEKNIGHT) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKKNIGHT))) {
        board[rcheck][ccheck] = 1;
    }
    rcheck = r + 1;
    ccheck = c + 2;
    if (rcheck < 8 && ccheck < 8 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEKNIGHT) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKKNIGHT))) {
        board[rcheck][ccheck] = 1;
    }
    rcheck = r - 1;
    ccheck = c + 2;
    if (rcheck >= 0 && ccheck < 8 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEKNIGHT) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKKNIGHT))) {
        board[rcheck][ccheck] = 1;
    }
    rcheck = r + 1;
    ccheck = c - 2;
    if (rcheck < 8 && ccheck >= 0 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEKNIGHT) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKKNIGHT))) {
        board[rcheck][ccheck] = 1;
    }
    rcheck = r - 1;
    ccheck = c - 2;
    if (rcheck >= 0 && ccheck >= 0 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEKNIGHT) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKKNIGHT))) {
        board[rcheck][ccheck] = 1;
    }
}
void PawnMoveCheck(struct square gboard[8][8], int r, int c, int board[8][8]) {
    memset(board, 0, sizeof(int) * 64);
    int rcheck, ccheck, side;
    if (gboard[r][c].cpiece->type == BLACKPAWN) {
        side = 0;
        rcheck = r + 1;
        ccheck = c;
    }
    else {
        side = 1;
        rcheck = r - 1;
        ccheck = c;
    }
    if (gboard[r][c].cpiece->moves == 0) {
        if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
        }
        if (!side) {
            rcheck++;
        }
        else {
            rcheck--;
        }
        if (gboard[rcheck][ccheck].cpiece == NULL && ((side == 0 && board[rcheck - 1][ccheck] == 1) || (side == 1 && board[rcheck + 1][ccheck] == 1)) ) {
            board[rcheck][ccheck] = 1;
        }
    }
    else {
        if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
        }
    }
    if (!side) {
        rcheck = r + 1;
        ccheck = c;
    }
    else {
        rcheck = r - 1;
        ccheck = c;
    }
    if ((ccheck + 1 < 8 && gboard[rcheck][ccheck + 1].cpiece != NULL && gboard[rcheck][ccheck + 1].cpiece->type >= 7 && side == 1) || (ccheck + 1 < 8 && gboard[rcheck][ccheck + 1].cpiece != NULL && gboard[rcheck][ccheck + 1].cpiece->type < 7 && side == 0)) {
        board[rcheck][ccheck + 1] = 1;
    }
    if ((ccheck - 1 >= 0 && gboard[rcheck][ccheck  - 1].cpiece != NULL && gboard[rcheck][ccheck - 1].cpiece->type >= 7 && side == 1) || (ccheck - 1 >= 0 && gboard[rcheck][ccheck  - 1].cpiece && gboard[rcheck][ccheck - 1].cpiece->type < 7 && side == 0)) {
        board[rcheck][ccheck - 1] = 1;
    }
}
void BishopMoveCheck(struct square gboard[8][8], int r, int c, int board[8][8]) {
    int rcheck, ccheck;
    memset(board, 0, sizeof(int) * 64);
    if (r < 7 && c < 7) {
        rcheck = r + 1;
        ccheck = c + 1;
        while (rcheck < 8 && ccheck < 8) {
          if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEBISHOP) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKBISHOP))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEBISHOP) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKBISHOP)) {
            break;
          }
            board[rcheck][ccheck] = 1;
            rcheck++;
            ccheck++;
        }
    }
    if (r < 7 && c > 0) {
        rcheck = r + 1;
        ccheck = c - 1;
        while (rcheck < 8 && ccheck >= 0) {
          if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEBISHOP) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKBISHOP))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEBISHOP) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKBISHOP)) {
            break;
          }
            board[rcheck][ccheck] = 1;
            rcheck++;
            ccheck--;
        }
    }
    if (r > 0 && c < 7) {
        rcheck = r - 1;
        ccheck = c + 1;
        while (rcheck >= 0 && ccheck < 8) {
          if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEBISHOP) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKBISHOP))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEBISHOP) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKBISHOP)) {
            break;
          }
            board[rcheck][ccheck] = 1;
            rcheck--;
            ccheck++;
        }
    }
    if (r > 0 && c > 0) {
        rcheck = r - 1;
        ccheck = c - 1;
        while (rcheck >= 0 && ccheck >= 0) {
          if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEBISHOP) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKBISHOP))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEBISHOP) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKBISHOP)) {
            break;
          }
            board[rcheck][ccheck] = 1;
            rcheck--;
            ccheck--;
        }
    }
}
void QueenMoveCheck(struct square gboard[8][8], int r, int c, int board[8][8]) {
    int rcheck, ccheck;
    memset(board, 0, sizeof(int) * 64);
    if (r < 7) {
        rcheck = r + 1;
        ccheck = c;
        while (rcheck < 8) {
          if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKQUEEN))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKQUEEN)) {
            break;
          }
            board[rcheck][ccheck] = 1;
            rcheck++;
        }
    }
    if (r > 0) {
        rcheck = r - 1;
        ccheck = c;
        while (rcheck >= 0) {
            if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKQUEEN))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKQUEEN)) {
            break;
          }
            board[rcheck][ccheck] = 1;
            rcheck--;
        }
    }
    if (c < 7) {
        rcheck = r;
        ccheck = c + 1;
        while (ccheck < 8) {
            if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKQUEEN))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKQUEEN)) {
            break;
          }
            board[rcheck][ccheck] = 1;
            ccheck++;
        }
    }
    if (c > 0) {
        rcheck = r;
        ccheck = c - 1;
        while (ccheck >= 0) {
            if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKQUEEN))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKQUEEN)) {
            break;
          }
            board[rcheck][ccheck] = 1;
            ccheck--;
        }
    }
    if (r < 7 && c < 7) {
        rcheck = r + 1;
        ccheck = c + 1;
        while (rcheck < 8 && ccheck < 8) {
            if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKQUEEN))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKQUEEN)) {
            break;
          }
            board[rcheck][ccheck] = 1;
            rcheck++;
            ccheck++;
        }
    }
    if (r < 7 && c > 0) {
        rcheck = r + 1;
        ccheck = c - 1;
        while (rcheck < 8 && ccheck >= 0) {
            if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKQUEEN))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKQUEEN)) {
            break;
          }
            board[rcheck][ccheck] = 1;
            rcheck++;
            ccheck--;
        }
    }
    if (r > 0 && c < 7) {
        rcheck = r - 1;
        ccheck = c + 1;
        while (rcheck >= 0 && ccheck < 8) {
            if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKQUEEN))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKQUEEN)) {
            break;
          }
            board[rcheck][ccheck] = 1;
            rcheck--;
            ccheck++;
        }
    }
    if (r > 0 && c > 0) {
        rcheck = r - 1;
        ccheck = c - 1;
        while (rcheck >= 0 && ccheck >= 0) {
            if (gboard[rcheck][ccheck].cpiece == NULL) {
            board[rcheck][ccheck] = 1;
          }
          else if (gboard[rcheck][ccheck].cpiece != NULL && ((gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == BLACKQUEEN))) {
              board[rcheck][ccheck] = 1;
              break;
          }
          else if ((gboard[rcheck][ccheck].cpiece->type < 7 && gboard[r][c].cpiece->type == WHITEQUEEN) || (gboard[rcheck][ccheck].cpiece->type >= 7 && gboard[r][c].cpiece->type == BLACKQUEEN)) {
            break;
          }
            board[rcheck][ccheck] = 1;
            rcheck--;
            ccheck--;
        }
    }
}
void KingMovecheck(struct square gboard[8][8], int r, int c, int board[8][8]) {
    int rcheck, ccheck;
    memset(board, 0, sizeof(int) * 64);
    int side = gboard[r][c].cpiece != NULL && gboard[r][c].cpiece->type < 7;
    if (side == 1) {
    rcheck = r - 1;
    ccheck = c - 1;
    if (rcheck >= 0 && ccheck >= 0 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type >= 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r - 1;
    ccheck = c + 1;
    if (rcheck >= 0 && ccheck < 8 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type >= 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r - 1;
    ccheck = c;
    if (rcheck >= 0 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type >= 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r;
    ccheck = c - 1;
    if (ccheck >= 0 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type >= 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r;
    ccheck = c + 1;
    if (ccheck < 8 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type >= 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r + 1;
    ccheck = c - 1;
    if (rcheck < 8 && ccheck >= 0 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type >= 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r + 1;
    ccheck = c + 1;
    if (rcheck < 8 && ccheck < 8 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type >= 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r + 1;
    ccheck = c;
    if (rcheck < 8 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type >= 7))) {
      board[rcheck][ccheck] = 1;
    }
  }
  else {
    rcheck = r - 1;
    ccheck = c - 1;
    if (rcheck >= 0 && ccheck >= 0 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type < 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r - 1;
    ccheck = c + 1;
    if (rcheck >= 0 && ccheck < 8 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type < 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r - 1;
    ccheck = c;
    if (rcheck >= 0 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type < 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r;
    ccheck = c - 1;
    if (ccheck >= 0 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type < 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r;
    ccheck = c + 1;
    if (ccheck < 8 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type < 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r + 1;
    ccheck = c - 1;
    if (rcheck < 8 && ccheck >= 0 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type < 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r + 1;
    ccheck = c + 1;
    if (rcheck < 8 && ccheck < 8 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type < 7))) {
      board[rcheck][ccheck] = 1;
    }
    rcheck = r + 1;
    ccheck = c;
    if (rcheck < 8 && (gboard[rcheck][ccheck].cpiece == NULL || (gboard[rcheck][ccheck].cpiece != NULL && gboard[rcheck][ccheck].cpiece->type < 7))) {
      board[rcheck][ccheck] = 1;
    }
  }
}

void boardsetup(struct piece cpieces[32], struct square gboard[8][8]) {
    memset(cpieces, 0, sizeof(struct piece) * 32);
    memset(gboard, 0, sizeof(struct square) * 64);
    cpieces[0].type = BLACKROOK;
    cpieces[0].image = SDL_LoadBMP("img/blackrook.bmp");
    cpieces[0].movecheck = &RookMoveCheck;
    cpieces[1].type = BLACKKNIGHT;
    cpieces[1].image = SDL_LoadBMP("img/blackknight.bmp");
    cpieces[1].movecheck = &KnightMoveCheck;
    cpieces[2].type = BLACKBISHOP;
    cpieces[2].image = SDL_LoadBMP("img/blackbishop.bmp");
    cpieces[2].movecheck = &BishopMoveCheck;
    cpieces[3].type = BLACKQUEEN;
    cpieces[3].image = SDL_LoadBMP("img/blackqueen.bmp");
    cpieces[3].movecheck = &QueenMoveCheck;
    cpieces[4].type = BLACKKING;
    cpieces[4].image = SDL_LoadBMP("img/blackking.bmp");
    cpieces[4].movecheck = &KingMovecheck;
    cpieces[5].type = BLACKBISHOP;
    cpieces[5].image = SDL_LoadBMP("img/blackbishop.bmp");
    cpieces[5].movecheck = &BishopMoveCheck;
    cpieces[6].type = BLACKKNIGHT;
    cpieces[6].image = SDL_LoadBMP("img/blackknight.bmp");
    cpieces[6].movecheck = &KnightMoveCheck;
    cpieces[7].type = BLACKROOK;
    cpieces[7].image = SDL_LoadBMP("img/blackrook.bmp");
    cpieces[7].movecheck = &RookMoveCheck;
    cpieces[8].type = BLACKPAWN;
    cpieces[8].image = SDL_LoadBMP("img/blackpawn.bmp");
    cpieces[9].type = BLACKPAWN;
    cpieces[9].image = SDL_LoadBMP("img/blackpawn.bmp");
    cpieces[10].type = BLACKPAWN;
    cpieces[10].image = SDL_LoadBMP("img/blackpawn.bmp");
    cpieces[11].type = BLACKPAWN;
    cpieces[11].image = SDL_LoadBMP("img/blackpawn.bmp");
    cpieces[12].type = BLACKPAWN;
    cpieces[12].image = SDL_LoadBMP("img/blackpawn.bmp");
    cpieces[13].type = BLACKPAWN;
    cpieces[13].image = SDL_LoadBMP("img/blackpawn.bmp");
    cpieces[14].type = BLACKPAWN;
    cpieces[14].image = SDL_LoadBMP("img/blackpawn.bmp");
    cpieces[15].type = BLACKPAWN;
    cpieces[15].image = SDL_LoadBMP("img/blackpawn.bmp");
    cpieces[16].type = WHITEPAWN;
    cpieces[16].image = SDL_LoadBMP("img/whitepawn.bmp");
    cpieces[17].type = WHITEPAWN;
    cpieces[17].image = SDL_LoadBMP("img/whitepawn.bmp");
    cpieces[18].type = WHITEPAWN;
    cpieces[18].image = SDL_LoadBMP("img/whitepawn.bmp");
    cpieces[19].type = WHITEPAWN;
    cpieces[19].image = SDL_LoadBMP("img/whitepawn.bmp");
    cpieces[20].type = WHITEPAWN;
    cpieces[20].image = SDL_LoadBMP("img/whitepawn.bmp");
    cpieces[21].type = WHITEPAWN;
    cpieces[21].image = SDL_LoadBMP("img/whitepawn.bmp");
    cpieces[22].type = WHITEPAWN;
    cpieces[22].image = SDL_LoadBMP("img/whitepawn.bmp");
    cpieces[23].type = WHITEPAWN;
    cpieces[23].image = SDL_LoadBMP("img/whitepawn.bmp");
    cpieces[24].type = WHITEROOK;
    cpieces[24].image = SDL_LoadBMP("img/whiterook.bmp");
    cpieces[24].movecheck = &RookMoveCheck;
    cpieces[25].type = WHITEKNIGHT;
    cpieces[25].image = SDL_LoadBMP("img/whiteknight.bmp");
    cpieces[25].movecheck = &KnightMoveCheck;
    cpieces[26].type = WHITEBISHOP;
    cpieces[26].image = SDL_LoadBMP("img/whitebishop.bmp");
    cpieces[26].movecheck = &BishopMoveCheck;
    cpieces[27].type = WHITEQUEEN;
    cpieces[27].image = SDL_LoadBMP("img/whitequeen.bmp");
    cpieces[27].movecheck = &QueenMoveCheck;
    cpieces[28].type = WHITEKING;
    cpieces[28].image = SDL_LoadBMP("img/whiteking.bmp");
    cpieces[28].movecheck = &KingMovecheck;
    cpieces[29].type = WHITEBISHOP;
    cpieces[29].image = SDL_LoadBMP("img/whitebishop.bmp");
    cpieces[29].movecheck = &BishopMoveCheck;
    cpieces[30].type = WHITEKNIGHT;
    cpieces[30].image = SDL_LoadBMP("img/whiteknight.bmp");
    cpieces[30].movecheck = &KnightMoveCheck;
    cpieces[31].type = WHITEROOK;
    cpieces[31].image = SDL_LoadBMP("img/whiterook.bmp");
    cpieces[31].movecheck = &RookMoveCheck;
    int r, c;
    for (r = 0; r < 2; r++) {
        for (c = 0; c < 8; c++) {
            gboard[r][c].cpiece = &(cpieces[r * 8 + c]);
        }
    }
    for (r = 6; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            gboard[r][c].cpiece = &(cpieces[(r - 4) * 8 + c]);
        }
    }
    for (r = 0; r < 32; r++) {
        cpieces[r].image = SDL_ConvertSurface(cpieces[r].image, cpieces[r].image->format, 0);
        if (cpieces[r].type == WHITEPAWN || cpieces[r].type == BLACKPAWN) {
            cpieces[r].movecheck = &PawnMoveCheck;
        }
    }
}

int mouse_collision(int x, int y, SDL_Rect rect) {
    return x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h;
}

int main() {
    int sd = client_handshake();
//    int loginc = 0;
    const int WIDTH = 640;
    const int HEIGHT = 480;
    clock_t before = clock() - 50;
    struct user cuser;
    memset(&cuser, 0, sizeof(struct user));
    struct square gboard[8][8];
    char username[50];
    int board[8][8];
    struct piece cpieces[32];
    boardsetup(cpieces, gboard);
//    printf("cpieces: type %p\n", cpieces[5].image);
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window * window = SDL_CreateWindow("chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture * texture, * texture2;
    texture = NULL;
    texture2 = NULL;
    TTF_Font * font = TTF_OpenFont("./Roboto/Roboto-Medium.ttf", 25);
    SDL_Color color = {0, 0, 255};
    SDL_Surface * usertext, * passtext;
    struct move cmove;
    memset(&cmove, 0, sizeof(cmove));
    int w, h, r, c, rcur, ccur, side, valid, turn;
    side = 1;
    turn = 0;
    rcur = -1;
    ccur = -1;
    int quit = 0;
    int cw, ch;
    int logw, logh, regw, regh;

    SDL_Event event;
    SDL_Rect usernamebox, passwordbox;
    SDL_Rect passwordRect;
    passwordRect.x = 0;
    passwordRect.y = 150;
    passwordRect.w = w;
    passwordRect.h = 100;
    int selected = 0;
    SDL_Rect usernameRect;
    usernameRect.x = 0;
    usernameRect.y = 0;
    usernameRect.w = w;
    usernameRect.h = 100;
    SDL_Rect loginbut, registerbut;
    loginbut.w = 50;
    loginbut.h = 25;
    loginbut.x = w / 4 - loginbut.w;
    loginbut.y = 300;
    registerbut.w = 50;
    registerbut.h = 25;
    registerbut.x = 3 * w / 4 - registerbut.w;
    registerbut.y = 300;
    SDL_StartTextInput();
    while(!quit) {
        cw = w;
        ch = h;
        if (turn == 0 && cuser.logged == 2) {
            printf("waiting for move\n");
            int bytes = read(sd, &cmove, sizeof(struct move));
            printf("recieved move: %d\n", bytes);
            printf("gets here: %d\n", cmove.check);
            if (cmove.rcur == -1) {
                cuser.logged = 1;
                boardsetup(cpieces, gboard);
                continue;
            }
            else if (cmove.check == 1) {
                rcur = -1;
                ccur = -1;
                printf("cannot make this move you are in check\n");
                turn = 1;
                continue;
            }
            else if (cmove.check == 2) {
                cuser.logged = 1;
                boardsetup(cpieces, gboard);
                printf("YOU LOST\n");
                continue;
            }
            else if (cmove.check == 7) {
                cuser.logged = 1;
                boardsetup(cpieces, gboard);
                printf("YOU WON");
                continue;
            }
            gboard[cmove.r][cmove.c].cpiece = gboard[cmove.rcur][cmove.ccur].cpiece;
            gboard[cmove.rcur][cmove.ccur].cpiece = NULL;
            rcur = -1;
            ccur = -1;
            gboard[cmove.r][cmove.c].cpiece->moves += 1;
            if (cmove.check > 0) {
              gboard[cmove.r][cmove.c].cpiece->type = cmove.check;
              switch (gboard[cmove.r][cmove.c].cpiece->type) {
                case WHITEROOK:
                  gboard[cmove.r][cmove.c].cpiece->image = SDL_LoadBMP("img/whiterook.bmp");
                  gboard[cmove.r][cmove.c].cpiece->movecheck = &RookMoveCheck;
                  break;
                case WHITEBISHOP:
                  gboard[cmove.r][cmove.c].cpiece->image = SDL_LoadBMP("img/whitebishop.bmp");
                  gboard[cmove.r][cmove.c].cpiece->movecheck = &BishopMoveCheck;
                  break;
                case WHITEKNIGHT:
                  gboard[cmove.r][cmove.c].cpiece->image = SDL_LoadBMP("img/whiteknight.bmp");
                  gboard[cmove.r][cmove.c].cpiece->movecheck = &KnightMoveCheck;
                  break;
                case WHITEQUEEN:
                  gboard[cmove.r][cmove.c].cpiece->image = SDL_LoadBMP("img/whitequeen.bmp");
                  gboard[cmove.r][cmove.c].cpiece->movecheck = &QueenMoveCheck;
                  break;
                case BLACKROOK:
                  gboard[cmove.r][cmove.c].cpiece->image = SDL_LoadBMP("img/blackrook.bmp");
                  gboard[cmove.r][cmove.c].cpiece->movecheck = &RookMoveCheck;
                  break;
                case BLACKBISHOP:
                  gboard[cmove.r][cmove.c].cpiece->image = SDL_LoadBMP("img/blackbishop.bmp");
                  gboard[cmove.r][cmove.c].cpiece->movecheck = &BishopMoveCheck;
                  break;
                case BLACKKNIGHT:
                  gboard[cmove.r][cmove.c].cpiece->image = SDL_LoadBMP("img/blackknight.bmp");
                  gboard[cmove.r][cmove.c].cpiece->movecheck = &KnightMoveCheck;
                  break;
                case BLACKQUEEN:
                  gboard[cmove.r][cmove.c].cpiece->image = SDL_LoadBMP("img/blackqueen.bmp");
                  gboard[cmove.r][cmove.c].cpiece->movecheck = &QueenMoveCheck;
                  break;
              }
            }
            turn = 1;
        }
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (cuser.logged == 2) {
                        for (r = 0; r < 8; r++) {
                            if (cuser.logged == 1) {
                                break;
                            }
                            for (c = 0; c < 8; c++) {
                                if (rcur == -1 && ccur == -1 && mouse_collision(event.button.x, event.button.y, gboard[r][c].rect)) {
//                                    int i, z;
                                    if (gboard[r][c].cpiece != NULL && ((gboard[r][c].cpiece->type >= 7 && side == 0) || (gboard[r][c].cpiece->type < 7 && side == 1))) {
                                        printf("selected\n");
                                        gboard[r][c].cpiece->movecheck(gboard, r, c, board);
                                        rcur = r;
                                        ccur = c;
                                    }
//                                    for (i = 0; i < 8; i++) {
//                                        for (z = 0; z < 8; z++) {
//                                            printf("move[%d][%d]: %d\n", i, z, board[i][z]);
//                                        }
//                                    }
                                }
                                else if (board[r][c] == 1 && mouse_collision(event.button.x, event.button.y, gboard[r][c].rect)) {
                                    if (turn == 1) {
                                        cmove.rcur = rcur;
                                        cmove.ccur = ccur;
                                        cmove.r = r;
                                        cmove.c = c;
                                        cmove.check = 0;
                                        printf("sending move\n");
                                        if (side == 1 && gboard[rcur][ccur].cpiece != NULL && gboard[rcur][ccur].cpiece->type == WHITEPAWN && r == 0) {
                                          printf("white gets here\n");
                                          SDL_Event pawnUp;
                                          SDL_WaitEvent(&pawnUp);
                                          while(pawnUp.type != SDL_TEXTINPUT) {
                                            if (SDL_WaitEvent(&pawnUp)) {
                                              if (pawnUp.type == SDL_TEXTINPUT) {
                                                if (strcmp(pawnUp.text.text, "1") == 0) {
                                                  cmove.check = WHITEROOK;
                                                }
                                                else if (strcmp(pawnUp.text.text, "2") == 0) {
                                                  cmove.check = WHITEBISHOP;
                                                }
                                                else if (strcmp(pawnUp.text.text, "3") == 0) {
                                                  cmove.check = WHITEKNIGHT;
                                                }
                                                else if (strcmp(pawnUp.text.text, "4") == 0) {
                                                  cmove.check = WHITEQUEEN;
                                                }
                                              }
                                            }
                                          }
                                        }
                                        else if (side == 0 && gboard[rcur][ccur].cpiece != NULL && gboard[rcur][ccur].cpiece->type == BLACKPAWN && r == 7) {
                                          printf("black gets here\n");
                                          SDL_Event pawnUp;
                                          SDL_WaitEvent(&pawnUp);
                                          while (pawnUp.type != SDL_TEXTINPUT) {
                                            if (SDL_WaitEvent(&pawnUp)) {
                                              if (pawnUp.type == SDL_TEXTINPUT) {
                                                if (strcmp(pawnUp.text.text, "1") == 0) {
                                                  cmove.check = BLACKROOK;
                                                }
                                                else if (strcmp(pawnUp.text.text, "2") == 0) {
                                                  cmove.check = BLACKBISHOP;
                                                }
                                                else if (strcmp(pawnUp.text.text, "3") == 0) {
                                                  cmove.check = BLACKKNIGHT;
                                                }
                                                else if (strcmp(pawnUp.text.text, "4") == 0) {
                                                  cmove.check = BLACKQUEEN;
                                                }
                                              }
                                            }
                                          }
                                        }
                                        write(sd, &cmove, sizeof(struct move));
                                        printf("waiting for validation\n");
                                        read(sd, &valid, 4);
                                        printf("valid: %d\n", valid);
                                        if (valid == 1) {
                                            gboard[r][c].cpiece = gboard[rcur][ccur].cpiece;
                                            gboard[rcur][ccur].cpiece = NULL;
                                            rcur = -1;
                                            ccur = -1;
                                            gboard[r][c].cpiece->moves += 1;
                                        }
                                        else if (valid > 1) {
                                          gboard[r][c].cpiece = gboard[rcur][ccur].cpiece;
                                          gboard[rcur][ccur].cpiece = NULL;
                                          rcur = -1;
                                          ccur = -1;
                                          gboard[r][c].cpiece->moves += 1;
                                          gboard[r][c].cpiece->type = valid;
                                          switch (gboard[r][c].cpiece->type) {
                                            case WHITEROOK:
                                              gboard[r][c].cpiece->image = SDL_LoadBMP("img/whiterook.bmp");
                                              gboard[r][c].cpiece->movecheck = &RookMoveCheck;
                                              break;
                                            case WHITEBISHOP:
                                              gboard[r][c].cpiece->image = SDL_LoadBMP("img/whitebishop.bmp");
                                              gboard[r][c].cpiece->movecheck = &BishopMoveCheck;
                                              break;
                                            case WHITEKNIGHT:
                                              gboard[r][c].cpiece->image = SDL_LoadBMP("img/whiteknight.bmp");
                                              gboard[r][c].cpiece->movecheck = &KnightMoveCheck;
                                              break;
                                            case WHITEQUEEN:
                                              gboard[r][c].cpiece->image = SDL_LoadBMP("img/whitequeen.bmp");
                                              gboard[r][c].cpiece->movecheck = &QueenMoveCheck;
                                              break;
                                            case BLACKROOK:
                                              gboard[r][c].cpiece->image = SDL_LoadBMP("img/blackrook.bmp");
                                              gboard[r][c].cpiece->movecheck = &RookMoveCheck;
                                              break;
                                            case BLACKBISHOP:
                                              gboard[r][c].cpiece->image = SDL_LoadBMP("img/blackbishop.bmp");
                                              gboard[r][c].cpiece->movecheck = &BishopMoveCheck;
                                              break;
                                            case BLACKKNIGHT:
                                              gboard[r][c].cpiece->image = SDL_LoadBMP("img/blackknight.bmp");
                                              gboard[r][c].cpiece->movecheck = &KnightMoveCheck;
                                              break;
                                            case BLACKQUEEN:
                                              gboard[r][c].cpiece->image = SDL_LoadBMP("img/blackqueen.bmp");
                                              gboard[r][c].cpiece->movecheck = &QueenMoveCheck;
                                              break;
                                          }
                                        }
                                        else {
                                            printf("server says invalid move\n");
                                        }
                                        turn = 0;
                                    }
                                }
                                else if (mouse_collision(event.button.x, event.button.y, gboard[r][c].rect) && gboard[r][c].cpiece != NULL && ((gboard[r][c].cpiece->type >= 7 && side == 0) || (gboard[r][c].cpiece->type < 7 && side == 1))) {
                                    gboard[r][c].cpiece->movecheck(gboard, r, c, board);
                                    rcur = r;
                                    ccur = c;
                                    printf("selected\n");
                                }
                            }
                        }
                    }
                    else if (cuser.logged == 1 && mouse_collision(event.button.x, event.button.y, loginbut)) {
                        printf("joining queue\n");
                        write(sd, "queue", 6);
                        read(sd, &side, 4);
                        cuser.logged = 2;
                        if (side == 1) {
                            turn = 1;
                        }
                        else {
                            turn = 0;
                        }
                    }
                    else {
                        if (mouse_collision(event.button.x, event.button.y, usernameRect)) {
                            printf("username selected\n");
                            selected = 0;
                        }
                        else if (mouse_collision(event.button.x, event.button.y, passwordRect)) {
                            printf("password selected\n");
                            selected = 1;
                        }
                        else if (mouse_collision(event.button.x, event.button.y, loginbut)) {
                            printf("login\n");
                            cuser.rating = 0;
                            cuser.logged = 0;
                            write(sd, &cuser, sizeof(struct user));
                            read(sd, &(cuser.logged), 4);
                            if (!cuser.logged) {
                                printf("incorrect username or password\n");
                            }
                        }
                        else if (mouse_collision(event.button.x, event.button.y, registerbut)) {
                            printf("register\n");
                            cuser.logged = -1;
                            cuser.rating = 0;
                            write(sd, &cuser, sizeof(struct user));
                            read(sd, &(cuser.logged), 4);
                            if (cuser.logged == -1) {
                                printf("username is taken\n");
                            }
                        }
                    }
                    //printf("mouse clicked: %d\n", event.button.x);
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                //printf("released\n");
            }
            if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN) {
//                printf("key: %s\n", event.text.text);
                    if (cuser.logged == 0 || cuser.logged == -1) {
                        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                            printf("login\n");
                            cuser.rating = 0;
                            cuser.logged = 0;
                            write(sd, &cuser, sizeof(struct user));
                            read(sd, &(cuser.logged), 4);
                            if (!cuser.logged) {
                                printf("incorrect username or password\n");
                            }
                        }
                        if (selected == 0 && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && strlen(cuser.username) > 0) {
                            printf("doing this stuff\n");
                            cuser.username[strlen(cuser.username) - 1] = '\0';                        
                        }
                        if (selected == 1 && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && strlen(cuser.password) > 0) {
                            cuser.password[strlen(cuser.password) - 1] = '\0';                        
                        }
                        if (selected == 0 && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB) {
                            printf("tab\n");
                            selected = 1;
                        }
                        else if (selected == 1 && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB) {
                            selected = 0;
                        }
                        if (selected == 0 && event.type == SDL_TEXTINPUT && strlen(cuser.username) < 50) {
                            printf("change username\n");
                            strcat(cuser.username, event.text.text);
                        }
                        if (selected == 1 && event.type == SDL_TEXTINPUT && strlen(cuser.password) < 50) {
                            printf("password change\n");
                            strcat(cuser.password, event.text.text);
                        }
                    }
            }
        }
        SDL_GetWindowSize(window, &w, &h);
        //printf("w: %d, h: %d\n", rect.w, rect.h);
        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderClear(renderer);
        if (cuser.logged == 2) {
            if (cw != w || ch != h || clock() - before >= 50) {
                for (r = 0; r < 8; r++) {
                    for (c = 0; c < 8; c++) {
                        if (side == 0) {
                            gboard[r][c].rect.x = (7 - c)*w/8;
                            gboard[r][c].rect.y = (7 - r)*h/8;
                            gboard[r][c].rect.w = w/8;
                            gboard[r][c].rect.h = h/8;
                        }
                        else if (side == 1) {
                            gboard[r][c].rect.x = c*w/8;
                            gboard[r][c].rect.y = r*h/8;
                            gboard[r][c].rect.w = w/8;
                            gboard[r][c].rect.h = h/8;
                        }
                        if ((r + c) % 2 == 0) {
                            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        }
                        else {
                            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        }
                        SDL_RenderFillRect(renderer, &(gboard[r][c].rect));
                        if (gboard[r][c].cpiece != NULL) {
                            if (texture != NULL) SDL_DestroyTexture(texture);
                            texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                            SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                        }

                    }
                }
                before = clock();
                SDL_RenderPresent(renderer);

            }
        }
        else if (cuser.logged == 1) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            loginbut.x = w / 4 - loginbut.w;
            usertext = TTF_RenderText_Solid(font, "queue", color);
            if (texture != NULL) SDL_DestroyTexture(texture);
            texture = SDL_CreateTextureFromSurface(renderer, usertext);
            SDL_RenderFillRect(renderer, &loginbut);
            SDL_RenderCopy(renderer, texture, NULL, &loginbut);

            SDL_RenderPresent(renderer);
        }
        else {
            //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            //SDL_RenderClear(renderer);
            usertext = TTF_RenderText_Solid(font, cuser.username, color);
            passtext = TTF_RenderText_Solid(font, cuser.password, color);
            if (texture != NULL) SDL_DestroyTexture(texture);
            if (texture2 != NULL) SDL_DestroyTexture(texture2);
            texture = SDL_CreateTextureFromSurface(renderer, usertext);
            texture2 = SDL_CreateTextureFromSurface(renderer, passtext);
            SDL_QueryTexture(texture, NULL, NULL, &logw, &logh);
            SDL_QueryTexture(texture2, NULL, NULL, &regw, &regh);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            passwordRect.w = w;
            usernameRect.w = w;
            SDL_RenderFillRect(renderer, &usernameRect);
            SDL_RenderFillRect(renderer, &passwordRect);
            //printf("regw: %d, regh: %d, logw: %d, logh: %d\n", regw, regh, logw, logh);
            usernamebox.x = 0;
            usernamebox.y = 0;
            usernamebox.w = logw;
            usernamebox.h = logh;
            SDL_RenderCopy(renderer, texture, NULL, &usernamebox);
            passwordbox.x = 0;
            passwordbox.y = 150;
            passwordbox.w = regw;
            passwordbox.h = regh;
            SDL_RenderCopy(renderer, texture2, NULL, &passwordbox);
            registerbut.x = 3 * w / 4 - registerbut.w;
            loginbut.x = w / 4 - loginbut.w;
            usertext = TTF_RenderText_Solid(font, "login", color);
            passtext = TTF_RenderText_Solid(font, "register", color);
            if (texture != NULL) SDL_DestroyTexture(texture);
            if (texture2 != NULL) SDL_DestroyTexture(texture2);
            texture = SDL_CreateTextureFromSurface(renderer, usertext);
            texture2 = SDL_CreateTextureFromSurface(renderer, passtext);
            SDL_RenderFillRect(renderer, &loginbut);
            SDL_RenderFillRect(renderer, &registerbut);
            SDL_RenderCopy(renderer, texture, NULL, &loginbut);
            SDL_RenderCopy(renderer, texture2, NULL, &registerbut);

            SDL_RenderPresent(renderer);
//            SDL_RenderFillRect(renderer, &passwordbox);
        }
        /*
        for (r = 0; r < 8; r++) {
            for (c = 0; c < 8; c++) {
                printf("rect: [%d][%d], x: %d, y: %d\n", r, c, gboard[r][c].rect.x, gboard[r][c].rect.y);
            }
        }
        sleep(10);*/
        
        
        SDL_Delay(20);
    }
    for (r = 0; r < 32; r++) {
        SDL_FreeSurface(cpieces[r].image);
    }
    TTF_CloseFont(font);
    SDL_StopTextInput();
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_Quit();
    return 0;
/*
//    int sd = client_handshake();
    char ucommand[500];
//    int loginc = 0;
    printf("login or register? ");
    fgets(ucommand, 500, stdin);
    if (strcmp(ucommand, "login\n") == 0) {
        while (!loginc) {
            printf("username: \n");
            fgets((char *)cuser.username, 50, stdin);
            printf("password: \n");
            fgets((char *)cuser.password, 50, stdin);
            cuser.rating = 0;
            cuser.logged = 0;
            write(sd, &cuser, sizeof(struct user));
            read(sd, &loginc, 4);
            if (!loginc) {
                printf("incorrect username or password\n");
            }
        }
    }
    else if (strcmp(ucommand, "register\n") == 0) {
        loginc = -1;
        while (loginc == -1) {
            printf("username: \n");
            fgets((char *)cuser.username, 50, stdin);
            printf("password: \n");
            fgets((char *)cuser.password, 50, stdin);
            cuser.rating = 0;
            cuser.logged = -1;
            write(sd, &cuser, sizeof(struct user));
            read(sd, &loginc, 4);
            if (loginc == -1) {
                printf("username is taken\n");
            }
        }
    }
    
 //   while(1) {
 //       fgets(ucommand, 500, stdin);
 //       printf("gets here\n");
 //   }
    close(sd);
    printf("ending\n");
    return 0;
    */
}
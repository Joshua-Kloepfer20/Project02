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
#include <sys/select.h>
#include <time.h>

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
struct move {int rcur; int ccur; int r; int c; };
struct board {int board[8][8]; int white; int black; int active; };

int server_bind() {
  int sd = socket(AF_INET, SOCK_STREAM, 0);
  struct addrinfo * hints;
  struct addrinfo * results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, "1119", hints, &results);
  bind(sd, results->ai_addr, results->ai_addrlen);
  printf("waiting for client\n");
  listen(sd, 5);
  free(hints);
  freeaddrinfo(results);
  return sd;
  /*
  struct sockaddr_storage client_address;
  socklen_t sock_size = sizeof(client_address);
  int client = accept(sd, (struct sockaddr * ) &client_address, &sock_size);
  printf("client has connected\n");
  free(hints);
  freeaddrinfo(results);
  int f = fork();
  if (f) {
    while (1) {
      client = accept(sd, (struct sockaddr * ) &client_address, &sock_size);
      f = fork();
      if (!f) {
        return client;
      }
    }
  }
  else {
    return client;
  }
  */
}
void boardsetup(int board[8][8]) {
  memset(board, 0, sizeof(int) * 64);
    board[0][0] = BLACKROOK;
    board[0][1] = BLACKKNIGHT;
    board[0][2] = BLACKBISHOP;
    board[0][3] = BLACKQUEEN;
    board[0][4] = BLACKKING;
    board[0][5] = BLACKBISHOP;
    board[0][6] = BLACKKNIGHT;
    board[0][7] = BLACKROOK;
    board[1][0] = BLACKPAWN;
    board[1][1] = BLACKPAWN;
    board[1][2] = BLACKPAWN;
    board[1][3] = BLACKPAWN;
    board[1][4] = BLACKPAWN;
    board[1][5] = BLACKPAWN;
    board[1][6] = BLACKPAWN;
    board[1][7] = BLACKPAWN;
    board[7][0] = WHITEROOK;
    board[7][1] = WHITEKNIGHT;
    board[7][2] = WHITEBISHOP;
    board[7][3] = WHITEQUEEN;
    board[7][4] = WHITEKING;
    board[7][5] = WHITEBISHOP;
    board[7][6] = WHITEKNIGHT;
    board[7][7] = WHITEROOK;
    board[6][0] = WHITEPAWN;
    board[6][1] = WHITEPAWN;
    board[6][2] = WHITEPAWN;
    board[6][3] = WHITEPAWN;
    board[6][4] = WHITEPAWN;
    board[6][5] = WHITEPAWN;
    board[6][6] = WHITEPAWN;
    board[6][7] = WHITEPAWN;
}
void copy(int boarddest[8][8], int boardsrc[8][8]) {
  int r, c;
  for (r = 0; r < 8; r++) {
    for (c = 0; c < 8; c++) {
      boarddest[r][c] = boardsrc[r][c];
    }
  }
}

int check(int boardcheck[8][8], int side) {
  int movable[8][8];
  if (side == 1) {
    for (r = 0; r < 8; r++) {
      for (c = 0; c < 8; c++) {
        if (boardcheck[r][c] == WHITEKING) {
          break;
        }
      }
      if (boardcheck[r][c] == WHITEKING) {
        break;
      }
    }
    int move = KingMovecheck(boardcheck, side, movable, r, c);
    int attacked = ifattack(boardcheck, side, r, c);
    if (move && attacked) {
      return 1;
    }
    else if (!move && attacked) {
      return 2;
    }
    else {
      return 0;
    }
  }
  else {
    for (r = 0; r < 8; r++) {
      for (c = 0; c < 8; c++) {
        if (boardcheck[r][c] == WHITEKING) {
          break;
        }
      }
      if (boardcheck[r][c] == WHITEKING) {
        break;
      }
    }
    int move = KingMovecheck(boardcheck, side, movable, r, c);
    int attacked = ifattack(boardcheck, side, r, c);
    if (move && attacked) {
      return 1;
    }
    else if (!move && attacked) {
      return 2;
    }
    else {
      return 0;
    }
  }
}

int KingMovecheck(int boardcheck[8][8], int side, int movable[8][8], int r, int c) {
  int rcheck, ccheck;
  memset(movable, 0, sizeof(int) * 64);
  if (side == 1) {
    int z = r + 2;
    int x = c + 2
    rcheck = r - 1;
    ccheck = c - 1;
    if (rcheck < 0) {
      rcheck = 0;
    }
    if (ccheck < 0) {
      ccheck = 0;
    }
    for (rcheck = rcheck; rcheck < z && rcheck < 8) {
      for (ccheck = ccheck; ccheck < x && ccheck < 8) {
        if (boardcheck[rcheck][ccheck] != WHITEKING && boardcheck[rcheck][ccheck] < 7 && ifattack(boardcheck, side, rcheck, ccheck)) {
          movable[rcheck][ccheck] = 1;
        }
      }
    }
  }
  else {

  }
  for (r = 0; r < 8; r++) {
    for (c = 0; c < 8; c++) {
      if (movable[r][c] == 1) {
        return 1;
      }
    }
  }
  return 0;
}

int ifattack(int boardcheck, int side, int r, int c) {
  int rcheck, ccheck;
  if (side == 1) {
    if ()
  }
  else {

  }
}

int main() {

    int sd = server_bind();
    int fd = open("./USERS", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
      printf("error opening file\n");
    }
    srand(time(NULL));
    char input[1000];
    char msg[6];
    char logoreg[9];
    int loginc = 0;
    fd_set client;
    fd_set cur;
    struct user * cusers;
    int size = 10;
    int queuesize = 0;
    int * queue;
    queue = malloc(sizeof(int) * size);
    cusers = malloc(sizeof(struct user) * size);
    int maxfd = sd;
    FD_ZERO(&client);
    FD_SET(sd, &client);
    int i;
    int numgames, newgame;
    numgames = 10;
    newgame = 0;
    int boardcheck[8][8];
    struct board * games = malloc(sizeof(struct board) * numgames);
    struct move cmove;
    memset(games, 0, sizeof(struct board) * numgames);
    //int test = 0;
    while (1) {
      while (queuesize >= 2) {
        printf("queuesize: %d\n", queuesize);
        for (i = queuesize - 1; i >= 0; i -= 2) {
          newgame = 0;
          while (games[newgame].active == 1) {
            newgame++;
          }
          printf("new game: %d\n", newgame);
          if (newgame == numgames) {
            numgames = numgames + .1 * numgames;
            games = realloc(games, sizeof(struct board) * numgames);
          }
          if (rand() % 2 == 1) {
            memset(&(games[newgame].board), 0, sizeof(int) * 64);
            games[newgame].white = queue[i];
            games[newgame].black = queue[i - 1];
            games[newgame].active = 1;
            memset(&queue[i - 1], 0, 8);
            queuesize -= 2;
            printf("queuesize is now zero\n");
            cusers[games[newgame].white].logged = 2;
            cusers[games[newgame].black].logged = 2;
            int side = 1;
            write(games[newgame].white, &side, 4);
            side = 0;
            write(games[newgame].black, &side, 4);
          }
        }
      }
      cur = client;
      //printf("is test in?: %d\n", FD_ISSET(test, &cur));
      select(maxfd + 1, &cur, NULL, NULL, NULL);

      for (i = 0; i <= maxfd; i++) {

        if (FD_ISSET(i, &cur)) {
          if (i == sd) {
            struct sockaddr_storage client_address;
            socklen_t sock_size = sizeof(client_address);
            int clientfd = accept(sd, (struct sockaddr * ) &client_address, &sock_size);
            //test = clientfd;
            if (clientfd >= size - 1) {
              printf("size change\n");
              size = size + .1 * size;
              printf("'realloc size\n");
              cusers = realloc(cusers, size * sizeof(struct user));
              queue = realloc(queue, sizeof(int) * size);
            }
            FD_SET(clientfd, &client);
            cusers[clientfd].logged = 0;
            if (clientfd > maxfd) {
              maxfd = clientfd;
            }
          }
          else {
            if (cusers[i].logged == 0) {
              //printf("logged: %d\n", cusers[i].logged);
              if (!read(i, &(cusers[i]), sizeof(struct user))) {
                printf("client dc detected\n");
                FD_CLR(i, &client);
                close(i);
                continue;
              }
              //printf("logged: %d\n", cusers[i].logged);
              if (cusers[i].logged == 0) {
                struct user temp;
                if (cusers[i].username[strlen(cusers[i].username) - 1] == '\n') cusers[i].username[strlen(cusers[i].username) - 1] = '\0';
                if (cusers[i].password[strlen(cusers[i].password) - 1] == '\n') cusers[i].password[strlen(cusers[i].password) - 1] = '\0';
                lseek(fd, 0, SEEK_SET);
                while (read(fd, &temp, sizeof(struct user))) {
                  if (strcmp(cusers[i].username, temp.username) == 0 && strcmp((cusers[i]).password, temp.password) == 0) {
                    cusers[i].logged = 1;
                    cusers[i].rating = temp.rating;
                    break;
                  }
                }
                printf("logged: %d\n", cusers[i].logged);
                write(i, &(cusers[i].logged), 4);
              }

              else if (cusers[i].logged == -1) {
                struct user temp;
                read(sd, &(cusers[i]), sizeof(struct user));
                if (cusers[i].username[strlen(cusers[i].username) - 1] == '\n') cusers[i].username[strlen(cusers[i].username) - 1] = '\0';
                if (cusers[i].password[strlen(cusers[i].password) - 1] == '\n') cusers[i].password[strlen(cusers[i].password) - 1] = '\0';
                lseek(fd, 0, SEEK_SET);
                while (read(fd, &temp, sizeof(struct user))) {
                  if (strcmp(cusers[i].username, temp.username) == 0) {
                    cusers[i].logged = 1;
                    break;
                  }
                }
                if (cusers[i].logged == 1) {
                  cusers[i].logged = -1;
                  write(i, &(cusers[i].logged), 4);
                  cusers[i].logged = 0;
                  continue;
                }
                cusers[i].logged = 1;
                write(i, &(cusers[i].logged), 4);
                cusers[i].rating = 0;
                printf("username: %s\tpasword: %s\tlogged: %d\n", cusers[i].username, cusers[i].password, cusers[i].logged);
                lseek(fd, 0, SEEK_END);
                write(fd, &(cusers[i]), sizeof(struct user));
            }
            }
            else if (cusers[i].logged == 1) {
              int z = read(i, msg, 6);
              if (!z) {
                printf("client dc detected\n");
                FD_CLR(i, &client);
                close(i);
                continue;
              }
              queue[queuesize] = i;
              queuesize += 1;
            }
            else {
              int z = read(i, &cmove, sizeof(struct move));
              if (!z) {
                printf("client dc detected\n");
                FD_CLR(i, &client);
                close(i);
                for (z = 0; z < numgames; z++) {
                  if (games[z].active == 1) {
                    if (games[z].white == i) {
                      cmove.rcur = -1;
                      write(games[z].black, &cmove, sizeof(struct move));
                      games[z].active = 0;
                      cusers[games[z].black].logged = 1;
                    }
                    else if (games[z].black == i) {
                      cmove.rcur = -1;
                      write(games[z].white, &cmove, sizeof(struct move));
                      games[z].active = 0;
                      cusers[games[z].white].logged = 1;
                    }
                  }
                }
                continue;
              }
              for (z = 0; z < numgames; z++) {
                if (games[z].active == 1) {
                  if (games[z].white == i) {
                    copy(boardcheck, games[z].board);
                    boardcheck[cmove.r][cmove.c] = boardcheck[cmove.rcur][cmove.ccur];
                    boardcheck[cmove.rcur][cmove.ccur] = 0;
                    int isCheck = check(boardcheck, 0);
                    write(games[z].black, &cmove, sizeof(struct move));
                  }
                  else if (games[z].black == i) {
                    copy(boardcheck, games[z].board);
                    boardcheck[cmove.r][cmove.c] = boardcheck[cmove.rcur][cmove.ccur];
                    boardcheck[cmove.rcur][cmove.ccur] = 0;
                    int isCheck = check(boardcheck, 0);
                    write(games[z].white, &cmove, sizeof(struct move));
                  }
                }
              }
            }
          }
        }
      }
    }
    return 0;
}
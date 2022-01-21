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
struct move {int rcur; int ccur; int r; int c};
struct message {char username[50]; char messsage[50]; };
struct board {int board[8][8]; int whitesd; int blacksd; };

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

int main() {

    int sd = server_bind();
    int fd = open("./USERS", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
      printf("error opening file\n");
    }
    char input[1000];
    char logoreg[9];
    int loginc = 0;
    fd_set client;
    fd_set cur;
    struct user * cusers;
    int size = 10;
    cusers = malloc(sizeof(struct user) * size);
    int maxfd = sd;
    FD_ZERO(&client);
    FD_SET(sd, &client);
    int i;
    struct board cboard;
    memset(&cboard, 0, sizeof(struct board));
    boardsetup(cboard.board);
    //int test = 0;
    while (1) {

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
              cusers = realloc(cusers, size * sizeof(struct user));
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
              int z = read(i, &cusers[i], sizeof(struct user));
              if (!z) {
                printf("client dc detected\n");
                FD_CLR(i, &client);
                close(i);
                continue;
              }
            }
          }
        }
      }
    }
    return 0;
}
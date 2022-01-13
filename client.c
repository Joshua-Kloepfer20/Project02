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

struct user {char username[50]; char password[50]; int rating;};

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

int main() {

    int sd = client_handshake();
    struct user cuser;
    char ucommand[500];
    int loginc = 0;
    printf("login or register? ");
    fgets(ucommand, 500, stdin);
    if (strcmp(ucommand, "login\n") == 0) {
        while (!loginc) {
            printf("username: \n");
            fgets((char *)cuser.username, 50, stdin);
            printf("password: \n");
            fgets((char *)cuser.password, 50, stdin);
            write(sd, "login", 9);
            cuser.rating = 0;
            write(sd, &cuser, sizeof(struct user));
            read(sd, &loginc, 4);
        }
    }
    else if (strcmp(ucommand, "register\n") == 0) {
        printf("username: \n");
        fgets((char *)cuser.username, 50, stdin);
        printf("password: \n");
        fgets((char *)cuser.password, 50, stdin);
        write(sd, "register", 9);
        cuser.rating = 0;
        write(sd, &cuser, sizeof(struct user));
    }
    
    while(1) {
        fgets(ucommand, 500, stdin);
        printf("gets here\n");
    }

    return 0;
}
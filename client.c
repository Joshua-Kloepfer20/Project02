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
    char ucommand[500];
    printf("login or register? ");
    fgets(ucommand, 500, stdin);
    if (strcmp(ucommand, "login\n") == 0) {
        char username[50];
        char password[50];
        printf("username: ");
        fgets(username, 50, stdin);
        printf("password: ");
        fgets(password, 50, stdin);
        write(sd, "login", 6);
        write(sd, username, 50);
        write(sd, password, 50);
    }
    else if (strcmp(ucommand, "register\n") == 0) {
        char username[50];
        char password[50];
        printf("username: \n");
        fgets(username, 50, stdin);
        printf("password: \n");
        fgets(password, 50, stdin);
        write(sd, "register", 9);
        write(sd, username, 50);
        write(sd, password, 50);
    }
    
    while(1) {
        fgets(ucommand, 500, stdin);
        printf("gets here\n");
    }

    return 0;
}
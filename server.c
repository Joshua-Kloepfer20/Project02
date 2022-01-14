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

int server_handshake() {
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
}

int main() {

    int sd = server_handshake();
    int fd = open("./USERS", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
      printf("error opening file\n");
    }
    struct user cuser;
    char input[1000];
    char logoreg[9];
    int loginc = 0;
    read(sd, logoreg, 9);
    if (strcmp(logoreg, "login") == 0) {
      while (!loginc) {
        struct user login;
        read(sd, &login, sizeof(struct user));
        if (login.username[strlen(login.username) - 1] == '\n') login.username[strlen(login.username) - 1] = '\0';
        if (login.password[strlen(login.password) - 1] == '\n') login.password[strlen(login.password) - 1] = '\0';
        printf("username: %s\npassword: %s\nrating: %d\n", login.username, login.password, login.rating);
        while (read(fd, &cuser, sizeof(struct user))) {
          printf("username: %s\npassword: %s\nrating: %d\n", cuser.username, cuser.password, cuser.rating);
          if (strcmp(cuser.username, login.username) == 0 && strcmp(cuser.password, login.password) == 0) {
            printf("we break\n");
            break;
          }
        }
        if (strcmp(cuser.username, login.username) == 0 && strcmp(cuser.password, login.password) == 0) {
          printf("user has logged in\n");
          loginc = 1;
          write(sd, &loginc, 4);
        }
        else {
          printf("wrong info\n");
          write(sd, &loginc, 4);
        }
      }
    }
    else if(strcmp(logoreg, "register") == 0) {
      //implement semaphore usage in case one or more users want to log in
      lseek(fd, 0, SEEK_END);
      read(sd, &cuser, sizeof(struct user));
      if (cuser.username[strlen(cuser.username) - 1] == '\n') cuser.username[strlen(cuser.username) - 1] = '\0';
      if (cuser.password[strlen(cuser.password) - 1] == '\n') cuser.password[strlen(cuser.password) - 1] = '\0';
      printf("username: %s\npassword: %s\nrating: %d\n", cuser.username, cuser.password, cuser.rating);
      write(fd, &cuser, sizeof(struct user));
    }

    

    //while(read(sd, input, 1000)) {

    //}
    printf("client had dc\n");
    return 0;
}
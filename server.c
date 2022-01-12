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

    int fd = open("./", O_CREAT | O_RDWR, 0644);

    int sd = server_handshake();
    char input[1000];
    char username[50];
    char password[50];
    char logoreg[9];
    read(fd, logoreg, 9);
    if (strcmp(logoreg, "login") == 0) {

    }
    else if(strcmp(logoreg, "register") == 0) {
        lseek(fd, 0, SEEK_END);
        struct user cuser;
        read(sd, &(cuser.username), 50);
        read(sd, &(cuser.username), 50);
        cuser.rating = 0;
        write(fd, &cuser, sizeof(struct user));
    }

    

    //while(read(sd, input, 1000)) {

    //}

    return 0;
}
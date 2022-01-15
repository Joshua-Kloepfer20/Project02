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

struct user {char username[50]; char password[50]; int rating; int logged; };
struct message {char username[50]; char messsage[50]; };

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
    
    const int WIDTH = 640;
    const int HEIGHT = 480;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface * image = SDL_LoadBMP("pawn.bmp");
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
    int w, h;
    int quit = 0;
    

    SDL_Event event;
    SDL_Rect rect[8][8];
    while(!quit) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    printf("mouse clicked: %d\n", event.button.x);
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                printf("released\n");
            }
        }
        SDL_GetWindowSize(window, &w, &h);
        //printf("w: %d, h: %d\n", rect.w, rect.h);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        int r;
        int c;
        for (r = 0; r < 8; r++) {
            for (c = 0; c < 8; c++) {
                rect[r][c].x = c*w/8;
                rect[r][c].y = r*h/8;
                rect[r][c].w = w/8;
                rect[r][c].h = h/8;
                if ((r + c) % 2 == 0) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                }
                SDL_RenderFillRect(renderer, &(rect[r][c]));
                SDL_RenderCopy(renderer, texture, NULL, &(rect[r][c]));
            }
        }
        /*
        for (r = 0; r < 8; r++) {
            for (c = 0; c < 8; c++) {
                printf("rect: [%d][%d], x: %d, y: %d\n", r, c, rect[r][c].x, rect[r][c].y);
            }
        }
        sleep(10);
        */
        SDL_RenderPresent(renderer);
    }
    SDL_FreeSurface(image);
    SDL_Quit();
    return 0;

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
}
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
struct message {char username[50]; char messsage[50]; };
struct square {SDL_Rect rect; SDL_Surface * image; };

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

void boardsetup(int board[8][8], int side) {
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
    
    const int WIDTH = 640;
    const int HEIGHT = 480;
    clock_t before = clock() - 50;
    struct user cuser;
    memset(&cuser, 0, sizeof(struct user));
    int board[8][8];
    boardsetup(board, 1);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture * texture;
    int w, h, r, c;
    int quit = 0;
    int cw, ch;

    SDL_Event event;
    struct square gboard[8][8];
    memset(gboard, 0, 64 * sizeof(struct square));
    SDL_Rect registerbox;
    registerbox.x = 150;
    registerbox.y = 0;
    registerbox.w = 100;
    registerbox.h = 100;
    SDL_Rect login;
    login.x = 0;
    login.y = 0;
    login.w = 100;
    login.h = 100;
    SDL_StartTextInput();
    SDL_SetTextInputRect(&login);
    while(!quit) {
        cw = w;
        ch = h;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (cuser.logged == 1) {
                        for (r = 0; r < 8; r++) {
                            for (c = 0; c < 8; c++) {
                                if (event.button.x > gboard[r][c].rect.x && event.button.x < gboard[r][c].rect.x + gboard[r][c].rect.w && event.button.y > gboard[r][c].rect.y && event.button.y < gboard[r][c].rect.y + gboard[r][c].rect.h) {
                                    printf("rect[%d][%d] was clicked\n", r, c);
                                }
                            }
                        }
                    }
                    //printf("mouse clicked: %d\n", event.button.x);
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                printf("released\n");
            }
//            if (event.type == SDL_TEXTINPUT) {
//                printf("key: %s\n", event.text.text);
//            }
        }
        SDL_GetWindowSize(window, &w, &h);
        //printf("w: %d, h: %d\n", rect.w, rect.h);
        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderClear(renderer);
        if (cuser.logged == 0) {
            if (cw != w || ch != h || clock() - before >= 50) {
                for (r = 0; r < 8; r++) {
                    for (c = 0; c < 8; c++) {
                        gboard[r][c].rect.x = c*w/8;
                        gboard[r][c].rect.y = r*h/8;
                        gboard[r][c].rect.w = w/8;
                        gboard[r][c].rect.h = h/8;
                        if ((r + c) % 2 == 0) {
                            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        }
                        else {
                            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        }
                        SDL_RenderFillRect(renderer, &(gboard[r][c].rect));
                        switch (board[r][c]) {
                            case WHITEPAWN:
                                gboard[r][c].image = SDL_LoadBMP("whitepawn.bmp");
                                texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].image);
                                SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                break;
                        }
                    }
                }
                before = clock();
                SDL_RenderPresent(renderer);

            }
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &login);
//            SDL_RenderFillRect(renderer, &registerbox);
        }
        /*
        for (r = 0; r < 8; r++) {
            for (c = 0; c < 8; c++) {
                printf("rect: [%d][%d], x: %d, y: %d\n", r, c, gboard[r][c].rect.x, gboard[r][c].rect.y);
            }
        }
        sleep(10);*/
        
        
        SDL_Delay(10);
    }
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            if (gboard[r][c].image != NULL) {
                SDL_FreeSurface(gboard[r][c].image);
            }
        }
    }
    SDL_StopTextInput();
    SDL_Quit();
    return 0;

    int sd = client_handshake();
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
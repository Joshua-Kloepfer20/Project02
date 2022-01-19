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
struct message {char username[50]; char messsage[50]; };
struct square {SDL_Rect rect; SDL_Surface * image; int img; };

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
    int board[8][8];
    boardsetup(board, 1);

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window * window = SDL_CreateWindow("chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture * texture, * texture2;
    TTF_Font * font = TTF_OpenFont("./Roboto/Roboto-Medium.ttf", 25);
    SDL_Color color = {0, 0, 255};
    SDL_Surface * usertext, * passtext;
    int w, h, r, c;
    int quit = 0;
    int cw, ch;
    int logw, logh, regw, regh;

    SDL_Event event;
    struct square gboard[8][8];
    memset(gboard, 0, 64 * sizeof(struct square));
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
                                if (mouse_collision(event.button.x, event.button.y, gboard[r][c].rect)) {
                                    printf("rect[%d][%d] was clicked\n", r, c);
                                }
                            }
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
            if (event.type == SDL_TEXTINPUT) {
                printf("key: %s\n", event.text.text);
                    if (selected == 0 && strlen(cuser.username) < 50) {
                        printf("change username\n");
                        strcat(cuser.username, event.text.text);
                    }
                    if (selected == 1 && strlen(cuser.password) < 50) {
                        printf("password change\n");
                        strcat(cuser.password, event.text.text);
                    }
            }
        }
        SDL_GetWindowSize(window, &w, &h);
        //printf("w: %d, h: %d\n", rect.w, rect.h);
        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderClear(renderer);
        if (cuser.logged == 1) {
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
                            case WHITEKING:
                                if (gboard[r][c].img != WHITEKING) {
                                gboard[r][c].image = SDL_LoadBMP("whiteking.bmp");
                                gboard[r][c].img = WHITEKING;
                                gboard[r][c].image = SDL_ConvertSurface(gboard[r][c].image, gboard[r][c].image->format, 0);
                                }
                                texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].image);
                                SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                break;
                            case WHITEQUEEN:
                                if (gboard[r][c].img != WHITEQUEEN) {
                                gboard[r][c].image = SDL_LoadBMP("whitequeen.bmp");
                                gboard[r][c].img = WHITEQUEEN;
                                gboard[r][c].image = SDL_ConvertSurface(gboard[r][c].image, gboard[r][c].image->format, 0);
                                }
                                texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].image);
                                SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                break;
                            case WHITEBISHOP:
                                if (gboard[r][c].img != WHITEBISHOP) {
                                gboard[r][c].image = SDL_LoadBMP("whitebishop.bmp");
                                gboard[r][c].img = WHITEBISHOP;
                                gboard[r][c].image = SDL_ConvertSurface(gboard[r][c].image, gboard[r][c].image->format, 0);
                                }
                                texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].image);
                                SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                break;
                            case WHITEKNIGHT:
                                if (gboard[r][c].img != WHITEKNIGHT) {
                                gboard[r][c].image = SDL_LoadBMP("whiteknight.bmp");
                                gboard[r][c].img = WHITEKNIGHT;
                                gboard[r][c].image = SDL_ConvertSurface(gboard[r][c].image, gboard[r][c].image->format, 0);
                                }
                                texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].image);
                                SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                break;
                            case WHITEROOK:
                                if (gboard[r][c].img != WHITEROOK) {
                                gboard[r][c].image = SDL_LoadBMP("whiterook.bmp");
                                gboard[r][c].img = WHITEROOK;
                                gboard[r][c].image = SDL_ConvertSurface(gboard[r][c].image, gboard[r][c].image->format, 0);
                                }
                                texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].image);
                                SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                break;
                            case WHITEPAWN:
                                if (gboard[r][c].img != WHITEPAWN) {
                                gboard[r][c].image = SDL_LoadBMP("whitepawn.bmp");
                                gboard[r][c].img = WHITEPAWN;
                                gboard[r][c].image = SDL_ConvertSurface(gboard[r][c].image, gboard[r][c].image->format, 0);
                                }
                                texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].image);
                                SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                break;
                            case BLACKKING:
                                if (gboard[r][c].img != BLACKKING) {
                                gboard[r][c].image = SDL_LoadBMP("blackking.bmp");
                                gboard[r][c].img = BLACKKING;
                                gboard[r][c].image = SDL_ConvertSurface(gboard[r][c].image, gboard[r][c].image->format, 0);
                                }
                                texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].image);
                                SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                break;
                            case BLACKQUEEN:
                                if (gboard[r][c].img != BLACKQUEEN) {
                                gboard[r][c].image = SDL_LoadBMP("blackqueen.bmp");
                                gboard[r][c].img = BLACKQUEEN;
                                gboard[r][c].image = SDL_ConvertSurface(gboard[r][c].image, gboard[r][c].image->format, 0);
                                }
                                texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].image);
                                SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                break;
                            case BLACKBISHOP:
                                if (gboard[r][c].img != BLACKBISHOP) {
                                gboard[r][c].image = SDL_LoadBMP("blackbishop.bmp");
                                gboard[r][c].img = BLACKBISHOP;
                                gboard[r][c].image = SDL_ConvertSurface(gboard[r][c].image, gboard[r][c].image->format, 0);
                                }
                                texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].image);
                                SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                break;
                            case BLACKKNIGHT:
                                if (gboard[r][c].img != BLACKKNIGHT) {
                                gboard[r][c].image = SDL_LoadBMP("blackknight.bmp");
                                gboard[r][c].img = BLACKKNIGHT;
                                gboard[r][c].image = SDL_ConvertSurface(gboard[r][c].image, gboard[r][c].image->format, 0);
                                }
                                texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].image);
                                SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                break;
                            case BLACKROOK:
                                if (gboard[r][c].img != BLACKROOK) {
                                gboard[r][c].image = SDL_LoadBMP("blackrook.bmp");
                                gboard[r][c].img = BLACKROOK;
                                gboard[r][c].image = SDL_ConvertSurface(gboard[r][c].image, gboard[r][c].image->format, 0);
                                }
                                texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].image);
                                SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                break;
                            case BLACKPAWN:
                                if (gboard[r][c].img != BLACKPAWN) {
                                gboard[r][c].image = SDL_LoadBMP("blackpawn.bmp");
                                gboard[r][c].img = BLACKPAWN;
                                gboard[r][c].image = SDL_ConvertSurface(gboard[r][c].image, gboard[r][c].image->format, 0);
                                }
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
            //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            //SDL_RenderClear(renderer);
            usertext = TTF_RenderText_Solid(font, cuser.username, color);
            passtext = TTF_RenderText_Solid(font, cuser.password, color);
            texture = SDL_CreateTextureFromSurface(renderer, usertext);
            texture2 = SDL_CreateTextureFromSurface(renderer, passtext);
            SDL_QueryTexture(texture, NULL, NULL, &logw, &logh);
            SDL_QueryTexture(texture2, NULL, NULL, &regw, &regh);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
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
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            if (gboard[r][c].image != NULL) {
                SDL_FreeSurface(gboard[r][c].image);
            }
        }
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
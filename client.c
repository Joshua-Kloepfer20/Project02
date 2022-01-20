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
struct piece {SDL_Surface * image; int type; int moves; /*for pawns*/ int ** (*movecheck)(struct square **, int, int); };
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

int ** RookMoveCheck(struct square gboard[8][8], int r, int c) {
    int rcheck, ccheck;
    for (rcheck = 0; rcheck < 8; rcheck++) {
        for (ccheck = 0; ccheck < 8; ccheck++) {
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
    cpieces[2].type = BLACKBISHOP;
    cpieces[2].image = SDL_LoadBMP("img/blackbishop.bmp");
    cpieces[3].type = BLACKQUEEN;
    cpieces[3].image = SDL_LoadBMP("img/blackqueen.bmp");
    cpieces[4].type = BLACKKING;
    cpieces[4].image = SDL_LoadBMP("img/blackking.bmp");
    cpieces[5].type = BLACKBISHOP;
    cpieces[5].image = SDL_LoadBMP("img/blackbishop.bmp");
    cpieces[6].type = BLACKKNIGHT;
    cpieces[6].image = SDL_LoadBMP("img/blackknight.bmp");
    cpieces[7].type = BLACKROOK;
    cpieces[7].image = SDL_LoadBMP("img/blackpawn.bmp");
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
    cpieces[25].type = WHITEKNIGHT;
    cpieces[25].image = SDL_LoadBMP("img/whiteknight.bmp");
    cpieces[26].type = WHITEBISHOP;
    cpieces[26].image = SDL_LoadBMP("img/whitebishop.bmp");
    cpieces[27].type = WHITEQUEEN;
    cpieces[27].image = SDL_LoadBMP("img/whitequeen.bmp");
    cpieces[28].type = WHITEKING;
    cpieces[28].image = SDL_LoadBMP("img/whiteking.bmp");
    cpieces[29].type = WHITEBISHOP;
    cpieces[29].image = SDL_LoadBMP("img/whitebishop.bmp");
    cpieces[30].type = WHITEKNIGHT;
    cpieces[30].image = SDL_LoadBMP("img/whiteknight.bmp");
    cpieces[31].type = WHITEROOK;
    cpieces[31].image = SDL_LoadBMP("img/whiterook.bmp");
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
//   int board[8][8];
    struct piece cpieces[32];
    boardsetup(cpieces, gboard);
//    printf("cpieces: type %p\n", cpieces[5].image);
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
            if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN) {
                printf("key: %s\n", event.text.text);
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
                        if (gboard[r][c].cpiece != NULL) {
                            switch (gboard[r][c].cpiece->type) {
                                case WHITEKING:
                                    texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                                    SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                    break;
                                case WHITEQUEEN:
                                    texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                                    SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                    break;
                                case WHITEBISHOP:
                                    texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                                    SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                    break;
                                case WHITEKNIGHT:
                                    texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                                    SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                    break;
                                case WHITEROOK:
                                    texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                                    SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                    break;
                                case WHITEPAWN:
                                    texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                                    SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                    break;
                                case BLACKKING:
                                    texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                                    SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                    break;
                                case BLACKQUEEN:
                                    texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                                    SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                    break;
                                case BLACKBISHOP:
                                    texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                                    SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                    break;
                                case BLACKKNIGHT:
                                    texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                                    SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                    break;
                                case BLACKROOK:
                                    texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                                    SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                    break;
                                case BLACKPAWN:
                                    texture = SDL_CreateTextureFromSurface(renderer, gboard[r][c].cpiece->image);
                                    SDL_RenderCopy(renderer, texture, NULL, &(gboard[r][c].rect));
                                    break;
                            }
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
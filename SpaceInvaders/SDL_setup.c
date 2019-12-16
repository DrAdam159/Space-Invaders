#include "lib.h"

void init(){
    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    }

    if(TTF_Init() == -1){
        fprintf(stderr, "TTF_Init Error\n");
    }
}

SDL_Window* createWin(){
    SDL_Window* win = SDL_CreateWindow("Space Invaders", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!win) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
    }
    return win;
}

SDL_Renderer* createRen(SDL_Window* win){
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        SDL_DestroyWindow(win);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
    }

    return ren;

}


SDL_Surface* loadSurface(SDL_Window* win, SDL_Renderer* ren, const char *path){
    SDL_Surface* surface = IMG_Load(path);
    if(!surface){
        fprintf(stderr, "SDL_CreateSurface Error: %s", SDL_GetError());
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }
    return surface;
}

SDL_Texture* texture(SDL_Window* win, SDL_Renderer* ren, SDL_Surface* texture){
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(ren, texture);
    SDL_FreeSurface(texture);
    if(!newTexture){
        fprintf(stderr, "SDL_CreateTexture Error: %s", SDL_GetError());
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }
    return newTexture;
}
TTF_Font* loadFont(const char *path, int size){
    TTF_Font* newFont = TTF_OpenFont(path, 16);
    if(!newFont){
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
    return newFont;
}


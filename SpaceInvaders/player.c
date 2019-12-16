#include "lib.h"

void endGame(SDL_Renderer* ren, TTF_Font* font, SDL_Color color, SDL_Window* win, enum EndGameResult type, int *end, int *leaving, int *endEnd){

    SDL_Event e;
    if(*end == true){
    bool loop = false;

    SDL_Surface* surfaceLeave = TTF_RenderText_Solid(font,">Press enter to play<", color);
    SDL_Surface* surfacePlay = TTF_RenderText_Solid(font,">Press escape to leave<", color);
    SDL_Rect play = {  WINDOW_WIDTH / 2 - 300, 600, 600, 100};
    SDL_Rect leave = {  WINDOW_WIDTH / 2 - 300, 700, 600, 100};

    SDL_Texture* gameTexture;
    SDL_Texture* alienTexture;
    SDL_Texture* leaveTexture = texture(win, ren, surfaceLeave);
    SDL_Texture* playTexture = texture(win, ren, surfacePlay);

       for(int i = 0; i < WINDOW_HEIGHT; i++){
        SDL_Rect animation = { 0, 0, WINDOW_WIDTH, i += 3 };
        SDL_RenderFillRect(ren, &animation);
        SDL_RenderPresent(ren);
       }
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        SDL_Rect r = { WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 - 200, 400, 200};
        if(*leaving != 1){
            SDL_RenderCopy(ren, leaveTexture, NULL,&play);
            SDL_RenderCopy(ren, playTexture, NULL,&leave);
        }


        if(type == END_GAME_LOST){
            SDL_Surface* gameLost = TTF_RenderText_Solid(font,"YOU DIED", color);
            gameTexture = texture(win, ren, gameLost);
            SDL_RenderCopy(ren, gameTexture, NULL,&r);

            SDL_Surface* alien = loadSurface(win, ren, "images/alien.png");
            alienTexture = texture(win, ren, alien);
            SDL_Rect alienRec= { WINDOW_WIDTH / 2 -40, r.y +190, 80, 80};
            SDL_RenderCopy(ren, alienTexture, NULL,&alienRec);
        }
        else if(type == END_GAME_WON){
            SDL_Surface* gameWon = TTF_RenderText_Solid(font,"YOU WON", color);
            gameTexture= texture(win, ren, gameWon);
            SDL_RenderCopy(ren, gameTexture, NULL,&r);
        }
        else if(*leaving == 1){
            SDL_Surface* gameQuit = TTF_RenderText_Solid(font,"Leaving game", color);
            gameTexture = texture(win, ren, gameQuit);
            SDL_RenderCopy(ren, gameTexture, NULL,&r);
            SDL_RenderPresent(ren);
            SDL_Delay(1000);
            *endEnd = 1;
            loop = true;
        }

        while (!loop){
            while (SDL_PollEvent(&e)){
                if (e.type == SDL_QUIT){
                    loop = true;
                }
                else if (e.type == SDL_KEYDOWN){
                        if (e.key.keysym.sym == SDLK_ESCAPE){
                            *endEnd = 1;
                             loop = true;
                        }
                else if (e.key.keysym.sym == SDLK_RETURN){
                            *endEnd = 0;
                            loop = true;
                }
            }
            SDL_RenderPresent(ren);
      }
      if(*leaving != 1){
        SDL_DestroyTexture(leaveTexture);
        SDL_DestroyTexture(playTexture);
      }
      SDL_DestroyTexture(gameTexture);
      SDL_DestroyTexture(alienTexture);
    }
}
}

bool menu(SDL_Renderer* ren, TTF_Font* font, SDL_Color color, SDL_Window* win){

        int rows = getRows("stats/stats.txt");
        int *statsFromFile = allocation(rows);
        int *lives = allocation(rows / 2); // pulku zabiraji zivoty, druhou zasahy
        int *hits = allocation(rows / 2);
        loadFromFile("stats/stats.txt", statsFromFile);
        separate(statsFromFile, lives, hits, rows);
        int max_hits = getHighest(rows / 2, hits);
        int max_lives = getHighest(rows / 2, lives);
        /*printf("MAX h: %d\n", max_hits);
        printf("MAX l: %d\n", max_lives);
        printf("%d\n", rows);
        for(int i = 0; i < rows / 2; i++){
            printf("Lives: %d, Hits: %d\n", lives[i], hits[i]);
        }*/

        SDL_Event e;
        bool loop = false;

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        SDL_Surface* ship = loadSurface(win, ren, "images/ship.png");
        SDL_Texture* shipTexture = texture(win, ren, ship);
        SDL_Rect shipLeft= { 10, 100, 80, 30};
        SDL_Rect shipRight= { WINDOW_WIDTH - 90, 100, 80, 30};
        SDL_RenderCopy(ren, shipTexture, NULL,&shipLeft);
        SDL_RenderCopy(ren, shipTexture, NULL,&shipRight);

        SDL_Rect r = { WINDOW_WIDTH / 2 - 300, 10, 600, 200};
        SDL_Surface* title = TTF_RenderText_Solid(font,"SPACE INVADERS", color);
        SDL_Texture* titleTexture = texture(win, ren, title);
        SDL_RenderCopy(ren, titleTexture, NULL,&r);

        SDL_Rect play = { r.x, r.y + r.h + 100, 600, 100};
        SDL_Color color2 = {0, 255, 0};
        SDL_Surface* playText = TTF_RenderText_Solid(font,">Press enter to play<", color2);
        SDL_Texture* playTexture = texture(win, ren, playText);
        SDL_RenderCopy(ren, playTexture, NULL,&play);

        char formattedStats1[50];
        float kdRatio = 0;
        if(max_lives < 3){
            kdRatio = abs(max_hits / max_lives - 3);
        }
        else{
            kdRatio = max_hits;
        }

        sprintf(formattedStats1, "BEST SCORE: %d | K/D: %.1f", max_hits, kdRatio);
        SDL_Rect stats = { WINDOW_WIDTH / 2 - 250, play.y + play.h + 100, 500, 50};
        SDL_Surface* scoreText = TTF_RenderText_Solid(font,formattedStats1, color2);
        SDL_Texture* scoreTexture = texture(win, ren, scoreText);
        SDL_RenderCopy(ren, scoreTexture, NULL,&stats);

        while (!loop){
            while (SDL_PollEvent(&e)){
                if (e.type == SDL_QUIT){
                    loop = true;
                    return false;
                }
                else if (e.type == SDL_KEYDOWN){
                        if (e.key.keysym.sym == SDLK_ESCAPE){
                            loop = true;
                            return false;
                        }
                        else if (e.key.keysym.sym == SDLK_RETURN){
                            loop = true;
                            return true;
                    }
                }
            }
            SDL_RenderPresent(ren);
      }
      free(statsFromFile);
      SDL_DestroyTexture(shipTexture);

    return false;
}

void control(int *quit, int *leaving, int *end, SDL_Rect *shipRect, Bullet ** bullets, int *endEnd){

    SDL_Event e;
    while (SDL_PollEvent(&e)){  //detekce klaves
            if (e.type == SDL_QUIT){
                    *quit = 1;
                    *leaving = 1;
                    *end = 1;

            }
            else if (e.type == SDL_KEYDOWN){
                    if (e.key.keysym.sym == SDLK_ESCAPE){ //ukonceni hry klavesou escape
                        *quit = 1;
                        *leaving = 1;
                        *end = 1;
                        *endEnd = 1;
                    }
                         /*pohyb pokud je hrac uvnitr okna*/
                    if(shipRect->x + shipRect->w < WINDOW_WIDTH && shipRect->x > 0 && shipRect->y + shipRect->h < WINDOW_HEIGHT && shipRect->y > 0){
                            if (e.key.keysym.sym == SDLK_LEFT){
                                shipRect->x -= 15;

                            }
                            else if (e.key.keysym.sym == SDLK_RIGHT){

                                shipRect->x += 15;
                            }
                            else if (e.key.keysym.sym == SDLK_UP){
                                shipRect->y -= 15;
                            }
                            else if (e.key.keysym.sym == SDLK_DOWN){
                                shipRect->y += 15;
                            }
                            else if (e.key.keysym.sym == SDLK_SPACE){  // strelba
                                addBullet(shipRect->x + shipRect->w/2, shipRect->y, 5, bullets); //priradi strele pozadovane souradnice
                            }
                    }

                    else if(shipRect->x + shipRect->w >= WINDOW_WIDTH){
                                shipRect->x -= 10;
                    }
                    else if(shipRect->x <= 0){
                                shipRect->x += 10;
                    }
                    else if(shipRect->y + shipRect->h >= WINDOW_HEIGHT){
                                shipRect->y -= 10;
                    }
                    else if(shipRect->y <= 0){
                                 shipRect->y += 10;
                    }
                }
            }

}



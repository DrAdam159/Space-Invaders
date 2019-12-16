#include "lib.h"



int main(int argc, char **argv)
{
/*------------------------------------------------Inicializace a nacteni-----------------------------*/
    init(); //inicializace SDL a TTF

    SDL_Window* win = createWin();
    SDL_Renderer* ren = createRen(win);

    SDL_Surface* ship = loadSurface(win, ren, "images/ship.png");
    SDL_Surface* bg = loadSurface(win, ren, "images/starfield.png");
    SDL_Surface* bullet = loadSurface(win, ren, "images/bullet.png");
    SDL_Surface* enemy = loadSurface(win, ren, "images/alien.png");
    SDL_Surface* enemyExplosion = loadSurface(win, ren, "images/explosion.png");
    SDL_Surface* groundSurface = loadSurface(win, ren, "images/ground.png");

    SDL_Texture* backgroundTexture = texture(win, ren, bg);
    SDL_Texture* shipTexture = texture(win, ren, ship);
    SDL_Texture* bulletTexture = texture(win, ren, bullet);
    SDL_Texture* enemyTexture = texture(win, ren, enemy);
    SDL_Texture* explosionTexture = texture(win, ren, enemyExplosion);
    SDL_Texture* groundTexture = texture(win, ren, groundSurface);


/*----------------------------------------------------------------------------------------------------*/

/*------------------------------------------------Nacteni fontu pisma---------------------------------*/
    TTF_Font* font = loadFont("font/font3.ttf", 16);
    SDL_Color color = {255, 255, 255};

    int hitCount = 0;
    int lives = 3;
    char formatted[50];
    char formattedLives[50];

    sprintf(formatted, "SCORE: %d", hitCount);
    sprintf(formattedLives, "LIVES: %d", lives);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, formatted, color);
    SDL_Surface* livesSurface = TTF_RenderText_Solid(font, formattedLives, color);

    SDL_Texture* textTexture = texture(win, ren, textSurface);
    SDL_Texture* livesTexture = texture(win, ren, livesSurface);

    SDL_Rect textRect= { 0, 0, 120, 40};
    SDL_Rect livesRect= { WINDOW_WIDTH - 300, 0, 120, 40};

/*----------------------------------------------------------------------------------------------------*/
    Bullet ** bullets;
    //printBullets(bullets, MAX_BULLETS);

/*--------------------------Struktura, ktera udrzuje pozici hracovy lode-----------------------------*/
    SDL_Rect *shipRect = (SDL_Rect*)malloc(sizeof(SDL_Rect));


/*----------------------------------------------------------------------------------------------------*/

/*--------------------------Struktura, ktera udrzuje pozici nepritele-----------------------------*/
    Enemy * enemies;
    //printEnemies(enemies);
/*----------------------------------------------------------------------------------------------------*/

    SDL_Rect groundRect= { 0, WINDOW_HEIGHT -80, WINDOW_WIDTH, 80};

    int direction = 1; //smer nepratelskeho pohybu
    int offsetX = 0;
    int offsetY = 0;
    int quit = 0; //false
    int en = 0; //false
    int leaving = 0; //false

    enum EndGameResult type;
    int endEnd  = 0;
do{
    //nulovani vsech hodnot pro novou hru
    endEnd  = 0;
    bullets = initBullets(MAX_BULLETS);
    shipRect->w = 80;
    shipRect->h = 40;
    shipRect->x = WINDOW_WIDTH / 2 - shipRect->w;
    shipRect->y = WINDOW_HEIGHT - shipRect->h - shipRect->w;
    enemies = initEnemy(5, 30, 30);

    sprintf(formattedLives, "LIVES: %d", lives);
    livesSurface = TTF_RenderText_Solid(font, formattedLives, color);
    livesTexture = texture(win, ren, livesSurface);
    SDL_RenderCopy(ren, livesTexture, NULL,&livesRect);

    sprintf(formatted, "SCORE: %d", hitCount);
    textSurface = TTF_RenderText_Solid(font, formatted, color);
    textTexture = texture(win, ren, textSurface);
    SDL_RenderCopy(ren, textTexture, NULL,&textRect);

    bool start = menu(ren, font, color, win);
    if(start == false){
            quit = 1;
            leaving = 1;
            en = 1;
    }


    while (!quit){      //gameloop
            control(&quit, &leaving, &en, shipRect, bullets, &endEnd); //ovladani

/*----------------------------Vycisteni okna, nastaveni jeho barvy a nahrani textur objektu-----------------------------*/
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

        SDL_RenderCopy(ren, backgroundTexture, NULL,NULL); //pozadi
        SDL_RenderCopy(ren, groundTexture, NULL,&groundRect); //pozadi
        SDL_RenderCopy(ren, shipTexture, NULL,shipRect); //hrac
        SDL_RenderCopy(ren, textTexture, NULL,&textRect); //skore
        SDL_RenderCopy(ren, livesTexture, NULL,&livesRect); //zivoty


/*----------------------------------------------------------------------------------------------------------------------*/


/*---------------------------------------Pridavani a odebirani strel + hitbox-------------------------------------------*/

    for(int i = 0; i < MAX_BULLETS; i++){
        if(bullets[i] != NULL){  //pokud je pozadovana strela aktivovana
                SDL_Rect rectBullet= { bullets[i]->x, bullets[i]->y, 8, 8 };
                SDL_RenderCopy(ren, bulletTexture, NULL, &rectBullet);
            }
    }

     for(int i = 0; i < MAX_BULLETS; i++){
        if(bullets[i] != NULL){
            bullets[i]->y -= bullets[i]->dx; //pohyb strely

            //kolize strely a nepritele

            for(int j = 0; j < 25; j++){
                  if( enemies[j].alive == 1 && bullets[i]->x > enemies[j].x && bullets[i]->x < enemies[j].x + 30 && bullets[i]->y > enemies[j].y && bullets[i]->y < enemies[j].y + 30){
                      enemies[j].alive = 0;

                      hitCount++;
                      if(hitCount == 25){
                        type = END_GAME_WON;
                        quit = 1;
                        en = 1;
                        break;
                      }
                      sprintf(formatted, "SCORE: %d", hitCount);
                      textSurface = TTF_RenderText_Solid(font, formatted, color);
                      textTexture = texture(win, ren, textSurface);
                      SDL_RenderCopy(ren, textTexture, NULL,&textRect);

                      removeBullet(i, bullets);
                      break;
                }

            }
        if(bullets[i] != NULL){
                 if(bullets[i]->y < -WINDOW_HEIGHT || bullets[i]->y > WINDOW_HEIGHT){ //ostranim strelu, pokud je mimo okno
                   removeBullet(i, bullets);
                  //  printf("Removed\n");
            }
        }

      }
    }

/*----------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------Kolize hrace s nepritelem-------------------------------------------*/

    for(int j = 0; j < 25; j++){
            if(enemies[j].alive == 1 && enemies[j].x >= shipRect->x && enemies[j].x + enemies[j].w  <= shipRect->x + shipRect->w && enemies[j].y >= shipRect->y && enemies[j].y + enemies[j].h   <= shipRect->y + shipRect->h){
                enemies[j].alive = 0;
                lives--;
                if(lives == 0){
                    type = END_GAME_LOST;
                    quit = true;
                    en = 1;
                    break;
                }
                sprintf(formattedLives, "LIVES: %d", lives);
                livesSurface = TTF_RenderText_Solid(font, formattedLives, color);
                livesTexture = texture(win, ren, livesSurface);
                SDL_RenderCopy(ren, livesTexture, NULL,&livesRect);
                shipRect->x = WINDOW_WIDTH / 2 - shipRect->w;
                shipRect->y = WINDOW_HEIGHT - shipRect->h - shipRect->w;
                break;
            }

        }
/*----------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------Kolize nepritele se zemi-------------------------------------------*/

    for(int j = 0; j < 25; j++){
            if(enemies[j].alive == 1 && enemies[j].y >= WINDOW_HEIGHT -150){
                type = END_GAME_LOST;
                quit = 1;
                en = 1;
                break;
            }

        }
/*----------------------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------Pohyb nepritele----------------------------------------------------*/

    int numberPerLine = 5;
    movement(enemies, numberPerLine, offsetX, offsetY);
    for(int y = 0; y < 5; y++){
        for(int x = 0; x < 5; x++){
            int index = (y * numberPerLine) + x;

            if(enemies[index].alive == 1){
                SDL_Rect rect = {enemies[index].x, enemies[index].y +40, enemies[index].w, enemies[index].h};
                SDL_RenderCopy(ren, enemyTexture, NULL,&rect);
            }

        }

    }


    if (enemies[4].x + enemies[0].w >= WINDOW_WIDTH ||enemies[0].x < 0){
        offsetY += 20;
        direction *= -1;
    }
    offsetX += direction * 2;



/*----------------------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------Update / refresh obrazovky pro nacteni zmen--------------------------------*/
        SDL_RenderPresent(ren);
/*----------------------------------------------------------------------------------------------------------------------*/




    }

    endGame(ren, font,color,win,type, &en, &leaving, &endEnd);

    if(type == END_GAME_LOST || type == END_GAME_WON){
        saveIntoFile(lives, hitCount, "stats/stats.txt");
    }

     quit = 0; //false
     en = 0; //false
     leaving = 0; //false
    offsetX = 0;
    offsetY = 0;
    hitCount = 0;
    lives = 3;
    freeBullets(bullets, MAX_BULLETS);
    free(enemies);
    type = EMPTY;
}while(endEnd == 0);



    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(shipTexture);
    SDL_DestroyTexture(bulletTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(explosionTexture);
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(livesTexture);
    SDL_DestroyTexture(groundTexture);

    //Hrac dosel na konec hry

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}

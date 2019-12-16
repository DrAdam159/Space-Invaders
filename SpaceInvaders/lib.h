#ifndef _MYLIB
#define _MYLIB

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_TTF.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define MAX_BULLETS 1000 //maximum strel v jednom okamziku na displeji
#define MAX_ENEMIES 25

typedef struct
{
  int x; //souradnice
  int y;
  int dx; //rychlost strely
} Bullet;

typedef struct
{
  int x; //souradnice
  int y;
  int dx; //rychlost pohybu
  int w;
  int h;
  int enemyCount;
  int alive;
} Enemy;

enum EndGameResult {
   END_GAME_WON,
   END_GAME_LEAVE,
   END_GAME_LOST,
   EMPTY
};


//inicialize SDL
void init();
SDL_Window* createWin();
SDL_Renderer* createRen(SDL_Window* win);
SDL_Surface* loadSurface(SDL_Window* win, SDL_Renderer* ren, const char *path);
SDL_Texture* texture(SDL_Window* win, SDL_Renderer* ren, SDL_Surface* texture);

TTF_Font* loadFont(const char *path, int size);

//funkce pro samotnou hru
void endGame(SDL_Renderer* ren, TTF_Font* font, SDL_Color color, SDL_Window* win, enum EndGameResult type, int *end, int *leaving, int *endEnd);
bool menu(SDL_Renderer* ren, TTF_Font* font, SDL_Color color, SDL_Window* win);
void control(int *quit, int *leaving, int *end, SDL_Rect *shipRect, Bullet ** bullets, int *endEnd);

//funkce pro strely
Bullet ** initBullets(const int maxCount);
void freeBullets(Bullet ** bullets, const int maxCount);
void printBullets(Bullet ** bullets, const int maxCount);
void addBullet(int x, int y, int dx, Bullet ** bullets);
void removeBullet(int i, Bullet ** bullets);

//funkce pro praci s neprateli
Enemy * initEnemy(int numberPerLine, int width, int height);
void printEnemies(Enemy * enemies);
void movement(Enemy * enemies, int numberPerLine, int offsetX, int offsetY);

//funkce zpracovavajici soubor
void saveIntoFile(int lives, int hitCount, const char *path);
int getRows(const char *path);
int *allocation(int rows);
void loadFromFile(const char *path, int *data);
void separate(int *data, int *lives, int *hits, int count);
int getHighest(int count, int *array);



#endif

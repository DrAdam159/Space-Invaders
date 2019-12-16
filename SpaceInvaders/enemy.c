#include "lib.h"

Enemy * initEnemy(int numberPerLine, int width, int height) {

    const int numberOfEnemies = numberPerLine * numberPerLine;
    Enemy * listOfEnemies = (Enemy*)malloc(sizeof(Enemy) * numberOfEnemies);

    if (listOfEnemies == NULL){
       return NULL;
    }

    for (int i = 0; i < numberOfEnemies; i++) {
        listOfEnemies[i].w = width;
        listOfEnemies[i].h = height;
        listOfEnemies[i].x = 0;
        listOfEnemies[i].y = 0;
        listOfEnemies[i].alive = 1;
    }

    return listOfEnemies;
}

void printEnemies(Enemy * enemies){

    for (int i = 0; i < 25; i++) {
        printf("%d. Enemy: (%d, %d), address: %p\n", i, enemies[i].x, enemies[i].y, &enemies[i]);
    }
}

void movement(Enemy * enemies, int numberPerLine, int offsetX, int offsetY) {
    int offsetBetween = 50;

    int index;

    for (int row = 0; row < numberPerLine; row++) {
        for (int col = 0; col < numberPerLine; col++) {
            index = (numberPerLine * row) + col;

            enemies[index].x = (enemies[index].w + offsetBetween) * col + offsetX;
            enemies[index].y = (enemies[index].h + offsetBetween) * row + offsetY;
        }
    }
}

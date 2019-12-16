#include "lib.h"

Bullet ** initBullets(const int maxCount) {
    Bullet ** listOfBullets = (Bullet**)malloc(sizeof(Bullet*) * maxCount);

    if (listOfBullets == NULL){
        return NULL;
    }

    int i;

    for (i = 0; i < maxCount; i++) {
        listOfBullets[i] = NULL;
    }

    return listOfBullets;
}

void freeBullets(Bullet ** bullets, const int maxCount) {
    if (bullets == NULL){
        return;
    }

    int i;

    for (i = 0; i < maxCount; i++) {
        if (bullets[i] != NULL) {
            free(bullets[i]);
        }
    }

    free(bullets);
}

void printBullets(Bullet ** bullets, const int maxCount) {
    int i;

    for (i = 0; i < maxCount; i++) {
        if (bullets[i] != NULL) {
            printf("%d. Bullet: (%d, %d), address: %p\n", i, bullets[i]->x, bullets[i]->y, bullets[i]);
        } else {
            printf("%d. Bullet: NULL\n", i);
        }
    }
}

void addBullet(int x, int y, int dx, Bullet ** bullets) {
    int found = -1;

    int i;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i] == NULL) {
            found = i;
            break;
        }
    }

    if (found != -1) {
        bullets[found] = (Bullet*)malloc(sizeof(Bullet));

        if (bullets[found] != NULL) {
            bullets[found]->x = x;
            bullets[found]->y = y;
            bullets[found]->dx = dx;
        }
    }
}

void removeBullet(int i, Bullet ** bullets) {
    if ((i < 0) || (i >= MAX_BULLETS)) return;

  //  printf("\n\n\n\n%p\n\n\n\n", bullets[i]);

    if (bullets[i] != NULL) {
        free(bullets[i]);
        bullets[i] = NULL;
    }

}

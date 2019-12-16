#include "lib.h"


void saveIntoFile(int lives, int hitCount, const char *path){

	FILE *f;
	f = fopen(path, "a");
	if(f == NULL){
        printf("Chyba pri zapisu do souboru\n");
		return;
	}
    fprintf(f, "%d\n", lives);
    fprintf(f, "%d\n", hitCount);

	fclose(f);
}

int getRows(const char *path){
    FILE *f;
    int c = 0;
    f = fopen(path, "r");
    if(f == NULL){
        printf("Chyba pri cteni ze souboru\n");
		return 0;
	}
    int count = 0;
    for (c = getc(f); c != EOF; c = getc(f)){

        if (c == '\n')
            count++;
    }
    fclose(f);

    return count;
}

int *allocation(int rows){
    int *ptr = malloc(rows * sizeof *ptr);
    if(ptr == NULL){
        printf("Chyba alokace\n");
        return NULL;
    }
    return ptr;
}

void loadFromFile(const char *path, int *data){

    FILE *f;
    int temp = 0;
    int i = 0;

	f = fopen(path, "r");
	if(f == NULL){
        printf("Chyba pri cteni ze souboru\n");
		return;
	}

	 while(fscanf(f,"%d",&temp) != EOF)
    {
         data[i] = temp;
         i++;
    }
    fclose(f);

}

void separate(int *data, int *lives, int *hits, int count){
    int h = 0;
    int l = 0;
    for(int i = 0; i < count; i++){
        if(i % 2 == 1){                 //informace o zivotech jsou kazdy druhy index
            hits[h] = data[i];
            h++;
        }
        else{
            lives[l] = data[i];
            l++;
        }
    }
}

int getHighest(int count, int *array){
    int max = 1;
    for(int i = 0; i < count; i++){
        if(array[i] > max){
            max = array[i];
        }
    }

    return max;
}




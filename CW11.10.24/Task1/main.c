#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void cube_rand(int ***arr, size_t x, size_t y, size_t z, int k_min, int k_max, int k_k) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                arr[i][j][k] = rand() % (k_max - k_min + 1) + k_min;
                if (arr[i][j][k] == k_k){goto FINAL;}
            }
        }
        FINAL:
    }
}

int ***cube_calloc(size_t x, size_t y, size_t z) {
    int ***arr = (int ***)calloc(x , sizeof(int **));
    if (arr == NULL){perror("First layer err"); return NULL; /*ERROR*/}

    for (int i = 0; i < x; i++) {
        arr[i] = (int **)calloc(y , sizeof(int *));
        if (arr[i] == NULL){
            perror("Second layer err"); /*ERROR*/
            /*FREE_ALL*/
            for (int j; j < i; j ++){free(arr[j]);} 
            free(arr);

            return NULL;
        }
        for (int j = 0; j < y; j++) {
            arr[i][j] = (int *)calloc(z , sizeof(int));
            if (arr[i][j] == NULL){
                perror("Third layer err"); return NULL; /*ERROR*/
                /*FREE_ALL*/
                for (int k; k < j; k++){free(arr[i][k]);}
                for (int k; k < i; k++){
                    for (int l; l < j; l++){free(arr[k][l]);}
                    free(arr[k]);
                }
                free(arr);
            }
        }
    }
    return arr;
}

void cube_free(int ***array, size_t x, size_t y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            free(array[i][j]); 
        }
        free(array[i]);  
    }
    free(array);  
}
void cube_print(int ***arr, size_t x, size_t y, size_t z) {
    for (int i = 0; i < x; i++) {
        printf("Слой %d:\n", i);
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {printf("%d ", arr[i][j][k]);}
            printf("\n");
        }
        printf("\n");
    }
}
int main() {
    size_t x_max = 3, y_max = 4, z_max = 5;
    int k_min=1, k_max=5, k=3;

    int ***arr = cube_calloc(x_max, y_max, z_max);

    srand(time(NULL));
    cube_rand(arr, x_max, y_max, z_max, k_min, k_max, k);

    cube_print(arr, x_max, y_max, z_max);

    cube_free(arr, x_max, y_max);

    return 0;
}

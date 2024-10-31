#include <stdio.h>
#include <stdlib.h>

struct FileData {
    FILE *fp;
    int current_value;
    int has_value;
};

int main(int argc, char *argv[]) {
    int num_files = argc - 1;
    struct FileData *files = malloc(num_files * sizeof(struct FileData));

    int active_files = 0;

    for (int i = 0; i < num_files; i++) {
        files[i].fp = fopen(argv[i + 1], "r");

        if (fscanf(files[i].fp, "%d", &files[i].current_value) == 1) {
            files[i].has_value = 1;
            active_files++;
        } else {
            files[i].has_value = 0;
            fclose(files[i].fp);
        }
    }

    while (active_files > 0) {

        int min_index = -1;
        for (int i = 0; i < num_files; i++) {
            if (files[i].has_value) {
                if (min_index == -1 || files[i].current_value < files[min_index].current_value) {
                    min_index = i;
                }
            }
        }
        printf("%d ", files[min_index].current_value);

        if (fscanf(files[min_index].fp, "%d", &files[min_index].current_value) == 1) {
            files[min_index].has_value = 1;
        } else {
            files[min_index].has_value = 0;
            active_files--;
            fclose(files[min_index].fp);
        }
    }

    printf("\n");

    free(files);

    return 0;
}

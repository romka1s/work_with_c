#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <string.h>

void PrintProbel(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}
int print_directory_tree(const char *dir_name, int depth) {
    struct dirent *entry;
    DIR *dp = opendir(dir_name);
    if (dp == NULL) {perror("Ошибка при открытии директории");return 1;}

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {continue;} // Пропускаем "." и ".."

        PrintProbel(depth); printf("%s\n", entry->d_name);

        char FilePath[1024];
        snprintf(FilePath, sizeof(FilePath), "%s/%s", dir_name, entry->d_name);

        struct stat fileStat;
        stat(FilePath, &fileStat);
        if (stat(FilePath, &fileStat) < 0) {perror("ERR access with FilePath");return 1;}
        if (S_ISDIR(fileStat.st_mode)) {print_directory_tree(FilePath, depth + 1);}
    }

    closedir(dp);
    return 0;
}

int main(int argc, char*argv[]){
    struct stat fileStat;
    char *FilePath = argv[0];
    if (stat(FilePath, &fileStat) < 0) {perror("ERR access with FilePath");return 1;}
    printf("Last address to File: %s", ctime(&fileStat.st_atime));
    if (S_ISREG(fileStat.st_mode)){printf("Default FILE\n\n");} 

    print_directory_tree(".", 0);
    return 0;
}
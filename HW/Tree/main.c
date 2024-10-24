#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

struct Node {
    char name[256];
    struct Node *child;
    struct Node *sibling;
};

struct Node* createNode(const char *name){
    struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
    if (newNode == NULL) {perror("err of malloc");exit(1);}

    strncpy(newNode->name, name, sizeof(newNode->name) - 1);

    newNode->name[sizeof(newNode->name) - 1] = '\0'; 
    newNode->child = NULL;
    newNode->sibling = NULL;
    return newNode;
}

struct Node* buildTree(const char *dir_name){
    struct Node *root = createNode(dir_name);
    struct dirent *entry;
    DIR *dp = opendir(dir_name);
    if (dp == NULL) {perror("err open directory");return root;}

    struct Node *lastChild = NULL;
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {continue;}

        char FilePath[1024];
        snprintf(FilePath, sizeof(FilePath), "%s/%s", dir_name, entry->d_name);

        struct stat fileStat;
        if (stat(FilePath, &fileStat) < 0) {perror("Err status of file");continue;}

        struct Node *child = createNode(entry->d_name);

        if (S_ISDIR(fileStat.st_mode)) {
            child->child = buildTree(FilePath);
        }

        if (lastChild == NULL){
            root->child = child;
        }
        else {
            lastChild->sibling = child;
        }
        lastChild = child;
    }

    closedir(dp);
    return root;
}

void printTree(struct Node *root, int level){
    if (root == NULL || root -> name == NULL) {return;}

    for (int i = 0; i < level; i++){
        printf("   ");
    }
    printf("%s\n", root->name);
    
    printTree(root->child, level + 1);
    printTree(root->sibling, level);
}

void freeTree(struct Node *root){
    if (root == NULL) {return;}

    freeTree(root->child);
    freeTree(root->sibling);
    free(root);
}
void writeTreeToFile(struct Node *root, FILE *file){
    if (root == NULL) return;

    fwrite(root, sizeof(struct Node), 1, file);
    writeTreeToFile(root->child, file);
    writeTreeToFile(root->sibling, file);
}

int main(int argc, char* argv[]){
    if (argc < 2){perror("not directory");exit(1);}

    //create Tree
    char *FilePath = argv[1];
    struct Node *root = buildTree(FilePath);

    //print Tree
    printf("Tree directory:\n");
    printTree(root, 0);

    //write Tree to bin file
    FILE *file = fopen("Tree.bin", "wb");
    if (file == NULL) {perror("err of fopen bin file to write");exit(1);}
    writeTreeToFile(root, file);
    fclose(file);

    // //Give Tree from bin file
    // FILE *file1 = fopen("Tree.bin", "rb");
    // if (file == NULL) {perror("err of fopen bin file to read "); exit(1);}
    // struct Node *r = giveTreeFromFile(file);

    // printf("Tree directory (from file):\n");
    // printTree(r, 0);

    freeTree(root);
    return 0;
}

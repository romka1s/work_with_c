#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

struct TreeNode{
    struct TreeNode *left, *right;
    int value;
};

struct TreeNode *CreateNode(int value){
    struct TreeNode * NewNode = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    NewNode -> left = NewNode -> right = NULL;
    NewNode -> value = value;
    return NewNode;
}

struct TreeNode *Insert(struct TreeNode * root, int value){
    if (root == NULL){
        return CreateNode(value);
    }
    if (value < root -> value){
        root -> left = Insert(root->left, value);
    }
    else {
        root -> right = Insert(root -> right, value);
    }
    return root;
}

void PrintTree(struct TreeNode * root){
    if (root != NULL){
        PrintTree(root->left);
        printf("%d ", root->value);
        PrintTree(root->right);
    }
}

int countNodes(struct TreeNode* root) {
    if (root == NULL)
        return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void inorderTraversal(struct TreeNode* root, int* array, int* index) {
    if (root == NULL)
        return;
    inorderTraversal(root->left, array, index);
    array[*index] = root->value;
    (*index)++;
    inorderTraversal(root->right, array, index);
}

int* treeToSortedArray(struct TreeNode* root, int* size) {
    *size = countNodes(root);
    if (*size == 0)
        return NULL;

    int* array = (int*)malloc(sizeof(int) * (*size));
    if (!array) {
        perror("Err of malloc");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    inorderTraversal(root, array, &index);
    return array;
}

void FreeTree(struct TreeNode * root){
    if (root != NULL){
        FreeTree(root->left);
        FreeTree(root->right);
        free(root);
    }
}

int main(void){
    FILE * file = fopen("./file.txt", "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    pid_t pid1, pid2;

    int arr1[2], arr2[2];
    if ((pipe(arr1) < 0) || (pipe(arr2) < 0)){
        perror("Pipe err");
        exit(1);
    }

    int value;
    int cnt = 0;
    while (fscanf(file, "%d", &value) == 1){
        if (cnt % 2 == 0)
            write(arr1[1], &value, sizeof(value));
        else
            write(arr2[1], &value, sizeof(value));
        cnt++;
    }
    fclose(file);
    close(arr1[1]);
    close(arr2[1]);

    if ((pid1 = fork()) < 0){
        perror("Err of fork");
        exit(1);
    }
    else if (pid1 == 0){
        struct TreeNode * tree1 = NULL;
        close(arr1[1]);
        close(arr2[0]); 
        close(arr2[1]);

        while (read(arr1[0], &value, sizeof(value)) > 0){
            tree1 = Insert(tree1, value);
        }
        close(arr1[0]);

        int size1;
        int* sortedArray1 = treeToSortedArray(tree1, &size1);

        if (pipe(arr1) < 0) {
            perror("Pipe err");
            exit(1);
        }
        write(arr1[1], sortedArray1, sizeof(int) * size1);
        close(arr1[1]);

        FreeTree(tree1);
        free(sortedArray1);
        exit(0);
    }

    if ((pid2 = fork()) < 0){
        perror("Err of fork");
        exit(1);
    }
    else if (pid2 == 0){
        struct TreeNode * tree2 = NULL;
        close(arr2[1]); 
        close(arr1[0]); 
        close(arr1[1]);

        while (read(arr2[0], &value, sizeof(value)) > 0){
            tree2 = Insert(tree2, value);
        }
        close(arr2[0]);

        int size2;
        int* sortedArray2 = treeToSortedArray(tree2, &size2);

        if (pipe(arr2) < 0) {
            perror("Pipe err");
            exit(1);
        }
        write(arr2[1], sortedArray2, sizeof(int) * size2);
        close(arr2[1]);

        FreeTree(tree2);
        free(sortedArray2);
        exit(0);
    }

    close(arr1[1]);
    close(arr2[1]);

    struct TreeNode * tree = NULL;

    while (read(arr1[0], &value, sizeof(value)) > 0){
        tree = Insert(tree, value);
    }
    close(arr1[0]);

    while (read(arr2[0], &value, sizeof(value)) > 0){
        tree = Insert(tree, value);
    }
    close(arr2[0]);

    PrintTree(tree);
    printf("\n");

    FreeTree(tree);
    return 0;
}

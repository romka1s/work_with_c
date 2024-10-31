#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{    
    struct Node *prev, *next;
    char *elem;
};

struct List
{
    struct Node *first, *last;
};

struct Node * CreateNode (char * data){
    struct Node * NewNode = (struct Node *)malloc(sizeof(struct Node));
    if (NewNode == NULL){
        perror("Error of malloc");
        exit(1);
    }

    NewNode->next = NULL;
    NewNode->prev = NULL;
    NewNode->elem = strdup(data);

    if (NewNode->elem == NULL){
        perror("Error of data");
        free(NewNode); // Освобождаем память узла
        exit(1);
    }
    return NewNode;
}

struct List * Init(int count){
    if (count <= 0){
        perror("Err Input");
        exit(1);
    }


    struct List * ps = (struct List *)malloc(sizeof(struct List));
    if (ps == NULL){
        perror("Err of malloc");
        exit(1);
    }

    ps->first = NULL;
    ps->last = NULL;

    struct Node* prevNode = NULL;
    for (int i = 0; i < count; i ++){
        char data[20];
        snprintf(data, sizeof(data), "Node %d", count - i);
        struct Node* q = CreateNode(data);

        if (i == 0){
            ps->first = q;
        }
        else{
            prevNode->next = q;
            q->prev = prevNode;
        }
        prevNode = q;
    }
    ps->last = prevNode;
    return ps;
}

void FreeList(struct List* list) {
    if (list == NULL) {
        return;
    }

    struct Node* current = list->first;

    while (current) {
        struct Node* next = current->next;

        free(current->elem);
        free(current);

        current = next;
    }

    free(list);
}

void PrintList(struct List *listr){
    struct Node * current = listr->first;
    while (current != NULL){
        printf("%s\n", current->elem);
        current = current->next;
    }
}

void process(struct List *pl, const char *str){
    struct Node *current = pl->first;
    struct List tmp = {NULL, NULL}; // Создаем вспомогательный список для узлов, больших str

    while (current != NULL){
        struct Node *NextCurrent = current->next;
        struct Node *PrevCurrent = current->prev;

        if (strcmp(current->elem, str) == 0){
            if (PrevCurrent != NULL)
                PrevCurrent->next = NextCurrent;
            else
                pl->first = NextCurrent; 

            if (NextCurrent != NULL)
                NextCurrent->prev = PrevCurrent;
            else
                pl->last = PrevCurrent; 

            free(current->elem);
            free(current);
        }
        else if (strcmp(current->elem, str) > 0){
            if (PrevCurrent != NULL)
                PrevCurrent->next = NextCurrent;
            else
                pl->first = NextCurrent; 

            if (NextCurrent != NULL)
                NextCurrent->prev = PrevCurrent;
            else
                pl->last = PrevCurrent; 

            current->prev = NULL;
            current->next = NULL;
            if (tmp.last == NULL){
                tmp.first = tmp.last = current;
            }
            else{
                tmp.last->next = current;
                current->prev = tmp.last;
                tmp.last = current;
            }
        }
        current = NextCurrent;
    }

    if (tmp.first != NULL){
        if (pl->last != NULL){
            pl->last->next = tmp.first;
            tmp.first->prev = pl->last;
            pl->last = tmp.last;
        }
        else{
            pl->first = tmp.first;
            pl->last = tmp.last;
        }
    }
}
int main(void){
    int count = 5; 
    struct List* myList = Init(count);
    
    char * str = "Node 3";
    printf("Исходный список:\n");
    PrintList(myList);

    process(myList, str);
    printf("\nПосле обработки:\n");
    PrintList(myList);

    FreeList(myList);
    return 0;
}

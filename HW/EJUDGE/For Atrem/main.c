#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000
#define INCORRECT_PARAMS "Incorrect parameters of command line\n"
#define FILE_ERROR "Can't open file %s\n"
#define DIAPASON_ERROR "Incorrect diapason\n"
#define UNKNOWN_ERROR "Unknown error\n"

// Definition of Tree Node for BST
typedef struct TreeNode {
    char *word;
    struct TreeNode *left, *right;
} TreeNode;

// Function to check if a character is a separator
int is_separator(char c) {
    return c == ';' || c == ',' || c == '.' || c == ' ' || c == '-' || c == ':' || c == '(' || c == ')';
}

// Function to create a new tree node
TreeNode* create_node(const char *word) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (!node) {
        fprintf(stderr, UNKNOWN_ERROR);
        exit(1);
    }
    node->word = strdup(word);
    if (!node->word) {
        fprintf(stderr, UNKNOWN_ERROR);
        exit(1);
    }
    node->left = node->right = NULL;
    return node;
}

// Function to insert a word into BST and check for duplicates
int insert(TreeNode **root, const char *word) {
    if (*root == NULL) {
        *root = create_node(word);
        return 0; // Not a duplicate
    }
    int cmp = strcmp(word, (*root)->word);
    if (cmp == 0) {
        return 1; // Duplicate found
    } else if (cmp < 0) {
        return insert(&((*root)->left), word);
    } else {
        return insert(&((*root)->right), word);
    }
}

// Function to free the BST
void free_tree(TreeNode *root) {
    if (root) {
        free(root->word);
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

// Function to process and remove duplicate words from a line
void remove_duplicate_words(char *line, const char *word_to_remove) {
    char *ptr = line;
    char word[MAX_LINE_LENGTH];
    int index = 0;
    int first_occurrence = 1;
    TreeNode *root = NULL;
    int first_word = 1; // Flag to handle spaces between words

    while (*ptr != '\0') {
        // Skip separators
        while (*ptr != '\0' && is_separator(*ptr)) {
            ptr++;
        }

        // Extract word
        index = 0;
        while (*ptr != '\0' && !is_separator(*ptr)) {
            word[index++] = *ptr++;
        }
        word[index] = '\0';

        if (index > 0) {
            // Check for word_to_remove
            if (word_to_remove && strcmp(word, word_to_remove) == 0) {
                if (first_occurrence) {
                    first_occurrence = 0;
                    // Insert into BST
                    if (!insert(&root, word)) {
                        // Output word
                        if (!first_word) printf(" ");
                        printf("%s", word);
                        first_word = 0;
                    }
                }
                // Else skip subsequent occurrences
            } else {
                // Check for duplicates using BST
                if (!insert(&root, word)) {
                    // Output word
                    if (!first_word) printf(" ");
                    printf("%s", word);
                    first_word = 0;
                }
            }
        }
    }
    printf("\n");
    free_tree(root);
}

// Function to remove consecutive duplicate lines
void remove_duplicate_lines(FILE *input, int start_line, int end_line) {
    char prev_line[MAX_LINE_LENGTH], curr_line[MAX_LINE_LENGTH];
    int line_number = 1;
    prev_line[0] = '\0';
    int in_range;

    while (fgets(curr_line, MAX_LINE_LENGTH, input)) {
        in_range = (line_number >= start_line) && (end_line == -1 || line_number <= end_line);
        if (in_range) {
            if (strcmp(curr_line, prev_line) != 0) {
                printf("%s", curr_line);
                strcpy(prev_line, curr_line);
            }
            // Else skip duplicate line
        } else {
            // Output line unchanged
            printf("%s", curr_line);
            prev_line[0] = '\0'; // Reset prev_line outside range
        }
        line_number++;
    }
}

// Function to process the file according to options
void process_file(FILE *input, const char *word_to_remove, int start_line, int end_line) {
    char line[MAX_LINE_LENGTH];
    int line_number = 1;
    int in_range;

    while (fgets(line, MAX_LINE_LENGTH, input)) {
        // Remove trailing newline character
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        in_range = (line_number >= start_line) && (end_line == -1 || line_number <= end_line);

        if (in_range) {
            remove_duplicate_words(line, word_to_remove);
        } else {
            // Output line unchanged
            printf("%s\n", line);
        }
        line_number++;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, INCORRECT_PARAMS);
        return 1;
    }

    const char *input_filename = NULL;
    const char *word_to_remove = NULL;
    int start_line = 1, end_line = -1;
    int option_w = 0, option_l = 0;
    int i = 1;

    // Parse input filename
    input_filename = argv[i];
    i++;

    // Parse options
    while (i < argc) {
        if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--worddup") == 0) {
            if (option_l) {
                fprintf(stderr, INCORRECT_PARAMS);
                return 1;
            }
            option_w = 1;
            i++;
            if (i >= argc) {
                fprintf(stderr, INCORRECT_PARAMS);
                return 1;
            }
            word_to_remove = argv[i];
            i++;
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--linedup") == 0) {
            if (option_w) {
                fprintf(stderr, INCORRECT_PARAMS);
                return 1;
            }
            option_l = 1;
            i++;
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--diapason") == 0) {
            i++;
            if (i >= argc) {
                fprintf(stderr, INCORRECT_PARAMS);
                return 1;
            }
            start_line = atoi(argv[i]);
            if (start_line <= 0) {
                fprintf(stderr, DIAPASON_ERROR);
                return 1;
            }
            i++;
            if (i < argc && isdigit(argv[i][0])) {
                end_line = atoi(argv[i]);
                if (end_line <= 0) {
                    fprintf(stderr, DIAPASON_ERROR);
                    return 1;
                }
                i++;
            }
        } else {
            fprintf(stderr, INCORRECT_PARAMS);
            return 1;
        }
    }

    if (end_line != -1 && start_line > end_line) {
        fprintf(stderr, DIAPASON_ERROR);
        return 1;
    }

    // Open input file
    FILE *input = fopen(input_filename, "r");
    if (input == NULL) {
        fprintf(stderr, FILE_ERROR, input_filename);
        return 1;
    }

    // Process file based on options
    if (option_w) {
        process_file(input, word_to_remove, start_line, end_line);
    } else if (option_l) {
        remove_duplicate_lines(input, start_line, end_line);
    } else {
        // No valid option provided
        fprintf(stderr, INCORRECT_PARAMS);
        fclose(input);
        return 1;
    }

    fclose(input);
    return 0;
}

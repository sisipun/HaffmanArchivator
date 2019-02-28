#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

#define ALPHABET_SIZE 256

struct node {
    int sym;
    size_t freq;
    struct haff_code *code;
    struct node *root;
    struct node *left;
    struct node *right;
};

struct haff_code {
    int len;
    int *code;
};

int *copyArray(int *array, int size) {
    int *copy = calloc(ALPHABET_SIZE, sizeof(int));

    for (int i = 0; i < size; i++) {
        *(copy + i) = *(array + i);
    }

    return copy;
}

void setCodes(struct node *curr_node, struct haff_code *prev_code) {
    if (curr_node->sym != -1) {
        curr_node->code = prev_code;
        return;
    }

    struct haff_code *left_code = malloc(sizeof(struct haff_code));
    left_code->len = prev_code->len;
    left_code->code = copyArray(prev_code->code, prev_code->len);
    *(left_code->code + left_code->len) = 0;
    left_code->len += 1;
    setCodes(curr_node->left, left_code);

    struct haff_code *right_code = malloc(sizeof(struct haff_code));
    right_code->len = prev_code->len;
    right_code->code = copyArray(prev_code->code, prev_code->len);
    *(right_code->code + right_code->len) = 1;
    right_code->len += 1;
    setCodes(curr_node->right, right_code);
}

void encode(FILE *input, FILE *output) {
    struct node **node_array = calloc(ALPHABET_SIZE, sizeof(struct node *));
    int current_alphabet_size = 0;
    int sym_count = 0;

    int sym;
    struct node *curr_node;
    while ((sym = getc(input)) != EOF) {
        curr_node = *(node_array + sym);
        if (!curr_node) {
            curr_node = malloc(sizeof(struct node));
            curr_node->sym = sym;
            curr_node->freq = 0;
            curr_node->code = 0;
            curr_node->left = 0;
            curr_node->right = 0;
            *(node_array + sym) = curr_node;
            current_alphabet_size++;
        }
        (*(node_array + sym))->freq += 1;
        sym_count++;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        curr_node = *(node_array + i);
        if (curr_node) {
            push(curr_node->freq, curr_node);
        }
    }

    while (get_queue_size() != 1) {
        struct node *first_min_node = pop();
        struct node *second_min_node = pop();
        struct node *new_node = malloc(sizeof(struct node));
        new_node->sym = -1;
        new_node->freq = first_min_node->freq + second_min_node->freq;
        new_node->left = first_min_node;
        new_node->right = second_min_node;
        first_min_node->root = new_node;
        second_min_node->root = new_node;
        push(new_node->freq, new_node);
    }

    struct node *root = pop();
    struct haff_code *code = malloc(sizeof(struct haff_code));
    code->len = 0;
    code->code = calloc(ALPHABET_SIZE, sizeof(int));
    setCodes(root, code);

    fwrite(&current_alphabet_size, sizeof(int), 1, output);
    fwrite(&sym_count, sizeof(int), 1, output);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        curr_node = *(node_array + i);
        if (curr_node) {
            fwrite(curr_node, sizeof(struct node), 1, output);
            fwrite(curr_node->code, sizeof(struct haff_code), 1, output);
        }
    }

    rewind(input);
    char write_code = 0;
    int code_size = 0;
    while ((sym = getc(input)) != EOF) {
        curr_node = *(node_array + sym);
        int len = curr_node->code->len;
        int *curr_code = curr_node->code->code;
        for (int i = 0; i < len; i++) {
            if (*(curr_code + i)) {
                write_code = (write_code << 1) | 1;
            } else {
                write_code = write_code << 1;
            }
            code_size++;
            if (code_size == 8) {
                fwrite(&write_code, sizeof(char), 1, output);
                write_code = 0;
                code_size = 0;
            }
        }
    }

    if (code_size != 0) {
        write_code <<= (8 - code_size);
        fwrite(&write_code, sizeof(char), 1, output);
    }

    free(node_array);
}

int *charToBytes(char c) {
    int *bytes = calloc(8, sizeof(int));
    for (int i = 0; i < 8; i++) {
        int code;
        if (c & (1 << i)) {
            code = 1;
        } else {
            code = 0;
        }
        bytes[7 - i] = code;
    }

    return bytes;
}

void decode(FILE *input, FILE *output) {
    struct node **node_array = calloc(ALPHABET_SIZE, sizeof(struct node *));
    int current_alphabet_size;
    int sym_count;

    fread(&current_alphabet_size, sizeof(int), 1, input);
    fread(&sym_count, sizeof(int), 1, input);
    for (int i = 0; i < current_alphabet_size; i++) {
        struct node *new_node = malloc(sizeof(struct node));
        struct haff_code *new_code = malloc(sizeof(struct haff_code));
        fread(new_node, sizeof(struct node), 1, input);
        fread(new_code, sizeof(struct haff_code), 1, input);
        new_node->code = new_code;
        *(node_array + new_node->sym) = new_node;
        push(new_node->freq, new_node);
    }

    while (get_queue_size() != 1) {
        struct node *first_min_node = pop();
        struct node *second_min_node = pop();
        struct node *new_node = malloc(sizeof(struct node));
        new_node->sym = -1;
        new_node->freq = first_min_node->freq + second_min_node->freq;
        new_node->left = first_min_node;
        new_node->right = second_min_node;
        first_min_node->root = new_node;
        second_min_node->root = new_node;
        push(new_node->freq, new_node);
    }
    struct node *root = pop();

    char buff;
    struct node *curr_root = root;
    while (sym_count) {
        fread(&buff, sizeof(char), 1, input);
        int *bytes = charToBytes(buff);
        for (int i = 0; i < 8; i++) {
            if (bytes[i]) {
                curr_root = curr_root->right;
            } else {
                curr_root = curr_root->left;
            }
            if (curr_root->sym != -1) {
                fwrite(&curr_root->sym, sizeof(char), 1, output);
                curr_root = root;
                sym_count--;
                if (!sym_count) {
                    break;
                }
            }
        }
    }
}

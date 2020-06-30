#ifndef ARRAY_H
#define ARRAY_H

#include "types.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    Int = 1,
    Float,
    Double,
    Char
} ArrayType;

typedef struct _node {
    void* item;
    struct _node* previous;
    struct _node* next;
} _node;

typedef struct {
    ArrayType type;
    int len;
    _node *start, *end;
} _array;

typedef _node* Node;
typedef _array* OArray;

typedef struct {
    void (*init)(OArray array);
    void (*destroy)(OArray array);

    int (*is_empty)(OArray array);
    int (*len)(OArray array);

    int (*unshift)(OArray array, void* item);
    int (*push)(OArray array, void* item);

    int (*shift)(OArray array);
    int (*pop)(OArray array);
} ArrayClass;

extern ArrayClass* Array;
void build_array();

void init(OArray array);
void destroy(OArray array);

int is_empty(OArray array);
int len(OArray array);

int unshift(OArray array, void* item); //insert at start
int push(OArray array, void* item); //insert at end

int shift(OArray array); //remover from start
int pop(OArray array); //remove from the end

#endif
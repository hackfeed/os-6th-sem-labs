#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define STACK_ERROR 2
#define ALLOCATION_ERROR 3

typedef struct entry
{
    char *dir;
    char *path;
    size_t level;
} entry_t;

typedef struct dirstack
{
    entry_t *arr;
    size_t capacity;
    size_t size;
} dirstack_t;

dirstack_t init();

void push(dirstack_t *const stack, entry_t elem);

entry_t pop(dirstack_t *const stack);

void free_stack(const dirstack_t *const stack);

#endif

#include "stack.h"

void push(dirstack_t *const stack, entry_t elem)
{
    if (++(*stack).size > stack->capacity)
    {
        entry_t *temp = realloc(stack->arr, sizeof(entry_t) * stack->size * 2);
        if (!temp)
        {
            exit(ALLOCATION_ERROR);
        }

        stack->arr = temp;
        stack->capacity = stack->size * 2;
    }

    stack->arr[stack->size - 1] = elem;
}

entry_t pop(dirstack_t *const stack)
{
    if (stack->size > 0)
    {
        return stack->arr[--(*stack).size];
    }

    fprintf(stderr, "Stack is empty.");
    exit(STACK_ERROR);
}

void free_stack(const dirstack_t *const stack)
{
    if (!stack)
    {
        exit(STACK_ERROR);
    }

    free(stack->arr);
}

dirstack_t init()
{
    dirstack_t stack =
        {
            .arr = NULL,
            .size = 0,
            .capacity = 10,
        };

    stack.arr = malloc(sizeof(entry_t) * 10);
    if (!stack.arr)
    {
        fprintf(stderr, "Error while allocation.");
        exit(ALLOCATION_ERROR);
    }

    return stack;
}

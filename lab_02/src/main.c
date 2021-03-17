#include <stdio.h>
#include "tree.h"

#define ARGS_ERROR 3

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "You should choose directory to be crawled.");

        return ARGS_ERROR;
    }

    dirtree(argv[1]);

    return OK;
}

#include "utils.h"

void log_entry(const char *const entry_name, size_t offset)
{
    fprintf(stdout, " ");

    for (size_t i = 0; i < offset; i++)
    {
        fprintf(stdout, " - ");
    }

    if (!offset)
    {
        fprintf(stdout, "> %s\n", entry_name);
    }
    else
    {
        fprintf(stdout, "%s\n", entry_name);
    }
}

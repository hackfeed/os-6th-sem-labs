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

char *join(const char *old_path, const char *const entry)
{
    int len = strlen(old_path);
    char *new_path = malloc(len + strlen(entry) + 2);

    if (!new_path)
    {
        exit(ALLOCATION_ERROR);
    }

    strcpy(new_path, old_path);
    new_path[len] = '/';
    strcpy(new_path + len + 1, entry);

    return new_path;
}

char *get_memstr(const char *const string, const size_t size)
{
    char *memstr = malloc(size + 1);

    if (!memstr)
    {
        exit(ALLOCATION_ERROR);
    }

    strcpy(memstr, string);

    return memstr;
}

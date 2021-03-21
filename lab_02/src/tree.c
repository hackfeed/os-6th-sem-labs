#include "tree.h"

void process_entry(dirstack_t *const stack, const size_t level, char *cur_entry)
{
    struct stat statbuf;

    if (lstat(cur_entry, &statbuf) < 0)
    {
        return;
    }

    if (S_ISDIR(statbuf.st_mode) != 0)
    {
        entry_t element = {cur_entry, realpath(cur_entry, NULL), level};
        push(stack, element);

        return;
    }

    log_entry(cur_entry, level);
}

void process_dir(dirstack_t *const stack, const entry_t cur_entry, DIR *dp, struct dirent *dirp)
{
    if ((dp = opendir(cur_entry.path)) != NULL)
    {
        chdir(cur_entry.path);
        while ((dirp = readdir(dp)) != NULL)
        {
            if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0)
            {
                process_entry(stack, cur_entry.level + 1, dirp->d_name);
            }
        }
    }
}

void dirtree(char *path)
{
    struct dirent *dirp = NULL;
    DIR *dp = NULL;
    size_t level = 0;
    entry_t cur_entry = {path, realpath(path, NULL), level};
    dirstack_t stack = init();

    push(&stack, cur_entry);

    while (stack.size)
    {
        cur_entry = pop(&stack);

        log_entry(cur_entry.dir, cur_entry.level);
        process_dir(&stack, cur_entry, dp, dirp);

        if (dp != NULL && closedir(dp) < 0)
        {
            fprintf(stderr, "Can't close directory %s\n", cur_entry.dir);
        }
    }

    free_stack(&stack);
}

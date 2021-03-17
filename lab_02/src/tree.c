#include "tree.h"

int process_entry(dirstack_t *const stack,
                  const size_t level,
                  const char *const cur_path,
                  const char *const cur_entry)
{
    struct stat statbuf;
    char *path = join(cur_path, cur_entry);

    if (lstat(path, &statbuf) < 0)
    {
        return LSTAT_ERROR;
    }

    if (S_ISDIR(statbuf.st_mode) != 0)
    {
        entry_t element = {path, level};
        push(stack, element);

        return OK;
    }

    free(path);
    log_entry(cur_entry, level);

    return OK;
}

void process_dir(dirstack_t *const stack, const entry_t cur_el, DIR *dp, struct dirent *dirp)
{
    if ((dp = opendir(cur_el.dir)) != NULL)
    {
        while ((dirp = readdir(dp)) != NULL)
        {
            if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0)
            {
                process_entry(stack, cur_el.level + 1, cur_el.dir, dirp->d_name);
            }
        }
    }
}

void dirtree(char *path)
{
    struct dirent *dirp = NULL;
    DIR *dp = NULL;
    size_t level = 0;
    char *fullpath = get_memstr(path, PATH_MAX + 1);
    entry_t cur_el = {fullpath, level};
    dirstack_t stack = init();

    push(&stack, cur_el);

    while (stack.size)
    {
        cur_el = pop(&stack);
        char *entry;

        if ((entry = strrchr(cur_el.dir, '/')))
        {
            log_entry(entry + 1, cur_el.level);
        }
        else
        {
            log_entry(cur_el.dir, cur_el.level);
        }

        process_dir(&stack, cur_el, dp, dirp);
        free(cur_el.dir);

        if (dp != NULL && closedir(dp) < 0)
        {
            fprintf(stderr, "Can't close directory %s\n", fullpath);
        }
    }

    free_stack(&stack);
}

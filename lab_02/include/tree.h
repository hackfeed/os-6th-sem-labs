#ifndef __TREE_H__
#define __TREE_H__

#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "stack.h"

#define LSTAT_ERROR 4

int process_entry(dirstack_t *const stack, const size_t level, const char *const cur_path, const char *const cur_file);

void process_dir(dirstack_t *const stack, const entry_t cur_el, DIR *dp, struct dirent *dirp);

void dirtree(char *path);

#endif

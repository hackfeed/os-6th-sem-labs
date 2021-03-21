#ifndef __TREE_H__
#define __TREE_H__

#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "stack.h"

void process_entry(dirstack_t *const stack, const size_t level, char *cur_entry);

void process_dir(dirstack_t *const stack, const entry_t cur_el, DIR *dp, struct dirent *dirp);

void dirtree(char *path);

#endif

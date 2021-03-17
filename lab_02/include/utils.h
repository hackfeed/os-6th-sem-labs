#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define OK 0
#define ALLOCATION_ERROR 2

void log_entry(const char *const entry_name, size_t offset);

char *join(const char *base, const char *const file);

char *get_memstr(const char *const string, const size_t size);

#endif

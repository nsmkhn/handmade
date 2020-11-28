#ifndef LIST_H
#define LIST_H

#include <stdio.h>

typedef struct list list_t;
typedef void (*print_func)(const void *const, FILE *);

list_t *
list_create(const int unsigned, const print_func);

void
list_destroy(list_t *const);

void
list_add(list_t *const, const void *const);

void
list_remove(list_t *const, const void *const);

int
list_contains(const list_t *const, const void *const);

void
list_print(const list_t *const, FILE *);

#endif //LIST_H
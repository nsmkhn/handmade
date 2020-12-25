#ifndef SET_H
#define SET_H

typedef int (*comparator)(const void *, const void *); // NOTE(niki): returns zero if values are equal,
// or -1/1 if the first argument is less/greater than the second, respectively

typedef struct set set_t;

set_t *
set_create(const int unsigned, const comparator);

void
set_destroy(set_t *const);

void
set_add(set_t *const, const void *const);

void
set_remove(set_t *const, const void *const);

int
set_contains(set_t *const, const void *const);

int unsigned
set_size(const set_t *const);

#endif //SET_H

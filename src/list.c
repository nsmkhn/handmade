#include "list.h"
#include "logging.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// TODO(niki): Optimize structs member ordering if possible.
// TODO(niki): Test coverage.
typedef struct node
{
    void *data;
    struct node *next;
} node_t;

struct list
{
    int unsigned type_size;
    node_t *head;
    print_func print;
};

list_t *
list_create(const int unsigned type_size, const print_func print)
{
    if (type_size == 0)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_message("Type size should be greater than zero", &info);
        exit(EXIT_FAILURE);
    }
    
    if (!print)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(NULL_POINTER, &info);
        exit(EXIT_FAILURE);
    }
    
    list_t *list = malloc(sizeof(list_t));
    if (!list)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(MALLOC_FAILURE, &info);
        exit(EXIT_FAILURE);
    }
    
    list->head = NULL;
    list->type_size = type_size;
    list->print = print;
    
    return(list);
}

void
list_destroy(list_t *const list)
{
    if (!list)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(NULL_POINTER, &info);
        exit(EXIT_FAILURE);
    }
    
    node_t *curr = list->head;
    node_t *next = list->head;
    while (curr)
    {
        next = curr->next;
        curr->next = NULL;
        free(curr->data);
        curr->data = NULL;
        free(curr);
        curr = next;
    }
    
    list->head = NULL;
    list->print= NULL;
    list->type_size = 0;
    free(list);
}

void
list_add(list_t *const list, const void *const data)
{
    if (!list || !data)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(NULL_POINTER, &info);
        exit(EXIT_FAILURE);
    }
    
    node_t *node = malloc(sizeof(node_t));
    if (!node)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(MALLOC_FAILURE, &info);
        exit(EXIT_FAILURE);
    }
    
    node->data = malloc(sizeof(list->type_size));
    if (!node->data)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(MALLOC_FAILURE, &info);
        exit(EXIT_FAILURE);
    }
    
    memcpy(node->data, data, list->type_size);
    node->next = list->head;
    list->head = node;
}

void
list_remove(list_t *const list, const void *const data)
{
    if (!list || !data)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(NULL_POINTER, &info);
        exit(EXIT_FAILURE);
    }
    
    node_t *curr = list->head;
    node_t *prev = list->head;
    while (curr && memcmp(curr->data, data, list->type_size) != 0)
    {
        prev = curr;
        curr = curr->next;
    }
    
    if (curr)
    {
        if (curr == list->head)
        {
            list->head = curr->next;
        }
        else
        {
            prev->next = curr->next;
        }
        curr->next = NULL;
        free(curr->data);
        free(curr);
    }
}

int
list_contains(const list_t *const list, const void *const data)
{
    if (!list || !data)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(NULL_POINTER, &info);
        exit(EXIT_FAILURE);
    }
    
    node_t *curr = list->head;
    while (curr && memcmp(curr->data, data, list->type_size) != 0)
    {
        curr = curr->next;
    }
    
    return(curr ? 1 : 0);
}

void
list_print(const list_t *const list, FILE *out)
{
    if (!list || !out)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(NULL_POINTER, &info);
        exit(EXIT_FAILURE);
    }
    
    node_t *curr = list->head;
    while (curr)
    {
        list->print(curr->data, out);
        curr = curr->next;
    }
    fprintf(out, "\n");
}
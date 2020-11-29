#include "list.h"
#include "logging.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node
{
    void *data;
    struct node *next;
} node_t;

struct list
{
    int unsigned data_size;
    node_t *head;
    print_func print;
};

list_t *
list_create(const int unsigned data_size, const print_func print)
{
    if (!print)
    {
        log_info_t info = { NULL_POINTER, ERROR, stderr, __FILENAME__, __FUNCTION__, __LINE__ };
        log(&info);
        exit(EXIT_FAILURE);
    }
    
    list_t *list = malloc(sizeof(list_t));
    if (!list)
    {
        log_info_t info = { MALLOC_FAILURE, ERROR, stderr, __FILENAME__, __FUNCTION__, __LINE__ };
        log(&info);
        exit(EXIT_FAILURE);
    }
    
    list->head = 0;
    list->data_size = data_size;
    list->print = print;
    
    return list;
}

void
list_destroy(list_t *const list)
{
    if (!list)
    {
        log_info_t info = { NULL_POINTER, ERROR, stderr, __FILENAME__, __FUNCTION__, __LINE__ };
        log(&info);
        exit(EXIT_FAILURE);
    }
    
    node_t *curr = list->head;
    node_t *next = list->head;
    while (curr)
    {
        next = curr->next;
        curr->next = 0;
        free(curr->data);
        curr->data = 0;
        free(curr);
        curr = next;
    }
    list->head = 0;
    list->print= 0;
    free(list);
}

void
list_add(list_t *const list, const void *const data)
{
    if (!list || !data)
    {
        log_info_t info = { NULL_POINTER, ERROR, stderr, __FILENAME__, __FUNCTION__, __LINE__ };
        log(&info);
        exit(EXIT_FAILURE);
    }
    
    node_t *node = malloc(sizeof(node_t));
    if (!node)
    {
        log_info_t info = { MALLOC_FAILURE, ERROR, stderr, __FILENAME__, __FUNCTION__, __LINE__ };
        log(&info);
        exit(EXIT_FAILURE);
    }
    
    node->data = malloc(sizeof(list->data_size));
    if (!node->data)
    {
        log_info_t info = { MALLOC_FAILURE, ERROR, stderr, __FILENAME__, __FUNCTION__, __LINE__ };
        log(&info);
        exit(EXIT_FAILURE);
    }
    
    memcpy(node->data, data, list->data_size);
    node->next = list->head;
    list->head = node;
}

void
list_remove(list_t *const list, const void *const data)
{
    if (!list || !data)
    {
        log_info_t info = { NULL_POINTER, ERROR, stderr, __FILENAME__, __FUNCTION__, __LINE__ };
        log(&info);
        exit(EXIT_FAILURE);
    }
    
    node_t *curr = list->head;
    node_t *prev = list->head;
    while (curr && memcmp(curr->data, data, list->data_size) != 0)
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
        curr->next = 0;
        free(curr->data);
        free(curr);
    }
}

int
list_contains(const list_t *const list, const void *const data)
{
    if (!list || !data)
    {
        log_info_t info = { NULL_POINTER, ERROR, stderr, __FILENAME__, __FUNCTION__, __LINE__ };
        log(&info);
        exit(EXIT_FAILURE);
    }
    
    node_t *curr = list->head;
    while (curr && memcmp(curr->data, data, list->data_size) != 0)
    {
        curr = curr->next;
    }
    
    return curr ? 1 : 0;
}

void
list_print(const list_t *const list, FILE *out)
{
    if (!list)
    {
        log_info_t info = { NULL_POINTER, ERROR, stderr, __FILENAME__, __FUNCTION__, __LINE__ };
        log(&info);
        exit(EXIT_FAILURE);
    }
    
    if (!list->head)
    {
        return;
    }
    
    node_t *curr = list->head;
    while (curr)
    {
        list->print(curr->data, out);
        curr = curr->next;
    }
    fprintf(out, "\n");
}
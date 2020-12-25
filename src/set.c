#include "set.h"
#include "logging.h"
#include <stdlib.h>
// TODO(niki): Optimize structs member ordering if possible.
// TODO(niki): Test coverage.
typedef struct node
{
    void *data;
    struct node *parent;
    struct node *left;
    struct node *right;
} node_t;

typedef struct set
{
    node_t *root;
    int unsigned type_size;
    int unsigned size;
    comparator compare;
} set_t;

set_t *
set_create(const int unsigned type_size, const comparator compare)
{
    if (type_size == 0)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_message("Data size should be greater than zero", &info);
        exit(EXIT_FAILURE);
    }
    
    if (!compare)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(NULL_POINTER, &info);
        exit(EXIT_FAILURE);
    }
    
    set_t *set = malloc(sizeof(set_t));
    if (!set)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(MALLOC_FAILURE, &info);
        exit(EXIT_FAILURE);
    }
    
    set->root = NULL;
    set->type_size = type_size;
    set->size = 0;
    set->compare = compare;
    
    return(set);
}

static void
set_destroy_node(node_t *const node)
{
    if (!node)
    {
        return;
    }
    
    set_destroy_node(node->left);
    set_destroy_node(node->right);
    
    if (node->parent)
    {
        if (node == node->parent->right)
        {
            node->parent->right = NULL;
        }
        else
        {
            node->parent->left = NULL;
        }
    }
    
    free(node->data);
    node->left = node->right = node->parent = node->data = NULL;
    free(node);
}

void
set_destroy(set_t *const set)
{
    if (!set)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(NULL_POINTER, &info);
        exit(EXIT_FAILURE);
    }
    
    set_destroy_node(set->root);
    
    set->root = NULL;
    set->compare = NULL;
    set->size = set->type_size = 0;
    free(set);
}

static node_t *
set_create_node(const set_t *const set, const void *const data, node_t *const parent)
{
    node_t *node = malloc(sizeof(node_t));
    if (!node)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(MALLOC_FAILURE, &info);
        exit(EXIT_FAILURE);
    }
    
    node->data = malloc(set->type_size);
    if (!node->data)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(MALLOC_FAILURE, &info);
        exit(EXIT_FAILURE);
    }
    
    memcpy(node->data, data, set->type_size);
    node->parent = parent;
    node->left = node->right = NULL;
    
    return(node);
}

static int
set_attempt_add(set_t *const set, const void *const data)
{
    node_t *walk = set->root;
    if (!walk)
    {
        set->root = set_create_node(set, data, NULL);
        return(1);
    }
    
    int c;
    while ((c = set->compare(walk->data, data)) != 0)
    {
        if (c < 0)
        {
            if (walk->right)
            {
                walk = walk->right;
            }
            else
            {
                walk->right = set_create_node(set, data, walk);
                return(1);
            }
        }
        else
        {
            if (walk->left)
            {
                walk = walk->left;
            }
            else
            {
                walk->left = set_create_node(set, data, walk);
                return(1);
            }
        }
    }
    
    return(0);
}

void
set_add(set_t *const set, const void *const data)
{
    if (!set || !data)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(NULL_POINTER, &info);
        exit(EXIT_FAILURE);
    }
    
    set->size = set_attempt_add(set, data) ? set->size + 1 : set->size;
}

static node_t *
set_locate_node(const set_t *const set, const void *const data)
{
    node_t *walk = set->root;
    int c;
    while (walk && (c = set->compare(walk->data, data)) != 0)
    {
        walk = c < 0 ? walk->right : walk->left;
    }
    
    return(walk);
}

static node_t *
set_locate_inorder_successor(const node_t *const node)
{
    node_t *successor = node->right;
    while (successor->left)
    {
        successor = successor->left;
    }
    
    return(successor);
}

static void
set_node_move_child(node_t *const node, node_t *const old_child, node_t *const new_child)
{
    if (node)
    {
        if (node->right == old_child)
        {
            node->right = new_child;
        }
        else
        {
            node->left = new_child;
        }
        
        if (new_child)
        {
            new_child->parent = node;
        }
    }
    else
    {
        if (new_child)
        {
            new_child->parent = NULL;
        }
    }
}

static void
set_remove_node(set_t *const set, node_t *const exile)
{
    if (!exile->left && !exile->right)
    {
        set_node_move_child(exile->parent, exile, NULL);
        set->root = exile == set->root ? NULL : set->root;
    }
    else if (!exile->left || !exile->right)
    {
        node_t *child = exile->left ? exile->left : exile->right;
        set_node_move_child(exile->parent, exile, child);
        set->root = exile == set->root ? child : set->root;
    }
    else
    {
        node_t *successor = set_locate_inorder_successor(exile);
        if (successor->parent != exile)
        {
            if (successor->right)
            {
                successor->right->parent = successor->parent;
                successor->parent->left = successor->right;
            }
            else
            {
                successor->parent->left = NULL;
            }
            exile->right->parent = successor;
            successor->right = exile->right;
        }
        exile->left->parent = successor;
        successor->left = exile->left;
        set_node_move_child(exile->parent, exile, successor);
        set->root = exile == set->root ? successor : set->root;
    }
    
    free(exile->data);
    exile->left = exile->right = exile->parent = exile->data = NULL;
    free(exile);
}

void
set_remove(set_t *const set, const void *const data)
{
    if (!set || !data)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(NULL_POINTER, &info);
        exit(EXIT_FAILURE);
    }
    
    node_t *exile = set_locate_node(set, data);
    if (exile)
    {
        set_remove_node(set, exile);
        --set->size;
    }
}

int
set_contains(set_t *set, const void *data)
{
    if (!set || !data)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(NULL_POINTER, &info);
        exit(EXIT_FAILURE);
    }
    
    return(set_locate_node(set, data) ? 1 : 0);
}

int unsigned
set_size(const set_t *const set)
{
    if (!set)
    {
        log_info_t info = { stderr, ERROR, __FILENAME__, __FUNCTION__, __LINE__ };
        log_event(NULL_POINTER, &info);
        exit(EXIT_FAILURE);
    }
    
    return(set->size);
}
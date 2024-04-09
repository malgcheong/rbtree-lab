#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  #ifdef SENTINEL
    node_t *nil;  // for sentinel
  #else
  
  #endif 
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);
void delete_recursive_node_t(rbtree *t, node_t * node);

node_t *rbtree_insert(rbtree *, const key_t);
void rbtree_insert_fixup(rbtree *, node_t *);
void rbtree_left_rotate(rbtree * t, node_t * z);
void rbtree_right_rotate(rbtree * t, node_t * z);




node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif  // _RBTREE_H_

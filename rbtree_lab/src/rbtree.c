#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *t = calloc(1, sizeof(*t));
  if (t == NULL) {
    return NULL;
  }

  node_t *nil = calloc(1, sizeof(*nil));
  if (nil == NULL) {
    free(t);
    return NULL;
  }

  nil->color = RBTREE_BLACK;
  nil->parent = nil;
  nil->left = nil;
  nil->right = nil;

  t->nil = nil;
  t->root = nil;

  return t;
}

static void free_node(node_t *x, node_t *nil) {
  if (x == nil) return;

  free_node(x->left, nil);
  free_node(x->right, nil);
  free(x);
}

void delete_rbtree(rbtree *t) {
  if (t == NULL) return;
  
  free_node(t->root, t->nil);
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

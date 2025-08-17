#include "rbtree.h"
#include <stdlib.h>

typedef enum { ROT_LEFT, ROT_RIGHT } rb_rot_t;

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

static void rotate(rbtree *t, node_t *x, rb_rot_t dir) {
  if (t == NULL || x == NULL) return;

  if (dir == ROT_LEFT) {
    if (x->right == t->nil) return;   // 회전할 서브트리 자체가 없음

    node_t *y = x->right;
  
    x->right = y->left;   // y의 왼쪽 서브 트리를 x의 오른쪽 서브 트리로 회전
    if (y->left != t->nil) {
      y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == t->nil) {
      t->root = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }

    y->left = x;
    x->parent = y;

  } else {    // ROT_RIGHT
    if (x->left == t->nil) return;

    node_t *y = x->left;
    
    x->left = y->right;
    if (y->right != t->nil) {
      y->right->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == t->nil) {
      t->root = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }

    y->right = x;
    x->parent = y;
  }
}

static void insert_fixup(rbtree *t, node_t *z) {
  if (t == NULL || z == NULL) return;

  while (z->parent->color == RBTREE_RED) {
    node_t *p = z->parent;
    node_t *g = p->parent;

    if (p == g->left) {   // 왼쪽 서브 트리
      node_t *u = g->right;
      if (u->color == RBTREE_RED) {
          // case1 삼촌 RED -> 색 뒤집기   
          p->color = RBTREE_BLACK;
          u->color = RBTREE_BLACK;
          g = RBTREE_RED;
          z = g;
      } else { 
        // 삼촌 BLACK
        if (z == p->right) {
          // case2 z가 안쪽 child   
          z = p;
          rotate(t, z, ROT_LEFT);
          p = z->parent;    // 회전 후 부모/조부모 갱신
          g = p->parent;
        }
        // case3 z가 바깥 child
        p->color = RBTREE_BLACK;
        g->color = RBTREE_RED;
        rotate(t, g, ROT_RIGHT);
      }
    } else {    // 오른쪽 서브 트리
      node_t *u = g->left;
      if (u->color == RBTREE_RED) {
        p->color = RBTREE_BLACK;
        u->color = RBTREE_BLACK;
        g->color = RBTREE_RED;
        z = g;
      } else {
        if (z == p->left) {
          z = p;
          rotate(t, z, ROT_RIGHT);
          p = z->parent;
          g = p->parent;
        }
        p->color = RBTREE_BLACK;
        g->color = RBTREE_RED;
        rotate(t, g, ROT_LEFT);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  if (t == NULL) return NULL;

  node_t *z = malloc(sizeof(*z));
  if (z == NULL) return NULL;

  z->key = key;
  z->color = RBTREE_RED;
  z->left = t->nil;
  z->right = t->nil;
  z->parent = t->nil;

  node_t *y = t->nil;   // 추적용 부모 후보
  node_t *x = t->root;
  
  while (x != t->nil) {
    y = x;
    if (z->key < x->key) {
      x = x->left;
    } else {
      x = x->right;  // 중복 키 우측으로, 과제가 multiset을 기대
    }
  }

  z->parent = y;
  if (y == t->nil) {
    t->root = z;
  } else if (z->key < y->key) {
    y->left = z;
  } else {
    y->right = z;
  }

  insert_fixup(t, z);

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

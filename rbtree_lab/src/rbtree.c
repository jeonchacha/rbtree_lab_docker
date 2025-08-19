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
          g->color = RBTREE_RED;
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

  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  if (t == NULL) return NULL;

  node_t *x = t->root;
  while (x != t->nil) {
    if (key == x->key) {
      return x;
    } 
    x = (key < x->key) ? x->left : x->right;
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  if (t == NULL || t->root == t->nil) return NULL;
  
  node_t *x = t->root;
  while (x->left != t->nil) {
    x = x->left;
  }
  
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  if (t == NULL || t->root == t->nil) return NULL;

  node_t *x = t->root;
  while (x->right != t->nil) {
    x = x->right;
  }

  return x;
}

static void erase_fixup(rbtree *t, node_t *x) {
  if (t == NULL || x == NULL) return;

  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      node_t *w = x->parent->right;
      // case 1 형제 색 BLACK으로 바꾸기 -> case 2~4
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotate(t, x->parent, ROT_LEFT);
        w = x->parent->right;
      }

      // case 2 double-black을 부모로 위임
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        // case 3 -> case 4로 전환
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate(t, w, ROT_RIGHT);
          w = x->parent->right;
        }
        // case 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        rotate(t, x->parent, ROT_LEFT);
        x = t->root;
      }
    } else {    // 좌우 대칭
      node_t *w = x->parent->left;
      // case 1 형제 색 BLACK으로 바꾸기 -> case 2~4
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotate(t, x->parent, ROT_RIGHT);
        w = x->parent->left;
      }

      // case 2 double-black을 부모로 위임
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        // case 3 -> case 4로 전환
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate(t, w, ROT_LEFT);
          w = x->parent->right;
        }
        // case 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rotate(t, x->parent, ROT_RIGHT);
        x = t->root;
      }
    }
  }
  
  x->color = RBTREE_BLACK;
}

static void transplant(rbtree *t, node_t *u, node_t *v) {
  if (t == NULL || u == NULL || v == NULL) return;

  if (u->parent == t->nil) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *z) {
  if (t == NULL || z == NULL) return 0;
  
  node_t *y = z;  // 실제로 삭제될 노드 후보
  color_t y_original_color = y->color;
  node_t *x = t->nil;   // 삭제 후 대체할 위치 추적 변수

  if (z->left == t->nil) {
    x = z->right;
    transplant(t, z, z->right);
  } else if (z->right == t->nil) {
    x = z->left;
    transplant(t, z, z->left);
  } else {
    node_t * succ = z->right;
    while (succ->left != t->nil) {
      succ = succ->left;
    }
    y = succ;    // 후계자 (오른쪽 서브트리의 최소)
    y_original_color = y->color;
    x = y->right;   // y는 왼쪽이 nil (최소니까)

    if (y->parent == z) {
      x->parent = y;    // x가 nil일 수도 있으므로 parent 지정 필요
    } else {    // y가 트리에서 더 아래쪽
      transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == RBTREE_BLACK) {
    erase_fixup(t, x);
  }

  free(z);

  return 0;
}

static void inorder_fill(const rbtree *t, const node_t *x, key_t *arr, size_t *idx) {
  if (x == t->nil) return;

  inorder_fill(t, x->left, arr, idx);
  arr[(*idx)++] = x->key;
  inorder_fill(t, x->right, arr, idx);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if (t == NULL || arr == NULL || n == 0) return 0;

  size_t idx = 0;
  inorder_fill(t, t->root, arr, &idx);

  return (int)idx;
}
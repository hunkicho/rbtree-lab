#include "rbtree.h"

#include <stdlib.h>
// 트리 생성 ------------------------------------------------------------------------------------------------

rbtree *new_rbtree() {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;

  p->root = nil;
  p->nil = nil;

  return p;
}

//-----------------------------------------

void delete_node(rbtree* t, node_t*node){
  // 왼쪽 자식이 있다면
  if (node->left != t->nil){
    delete_node(t,node->left);
  }
  // 오른쪽 자식이 있다면
  if (node->right != t->nil){
    delete_node(t,node->right);
  }
  // 해당 노드의 메모리 삭제
  free(node);
}

void delete_rbtree(rbtree *t) {
  // 빈 트리가 아니라면 루트값을 node삭제 함수로 보낸다.
  if(t->root != t->nil){
    delete_node(t,t->root);
	  free(t->nil);
  }else{ 
    free(t->root); // 빈 트리라면 root와 nil 그리고 트리 t를 차례로 지워준다.
  }
  free(t);
}

// 트리 원소 추가 -------------------------------------------------------------------------------------

void left_rotate(rbtree *t, node_t *x) {
  node_t *y;
  y = x->right;
  x->right = y->left;
  if(y->left != t->nil){
    y->left->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil){
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
 }

void right_rotate(rbtree *t, node_t *x) {
  node_t *y;
  y = x->left;
  x->left = y->right;
  if(y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil){
    t->root = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
 }

void insert_fixup(rbtree *t, node_t *z) {
  node_t *y;
  while(z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left){
      y = z->parent->parent->right;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if(z == z->parent->right){
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else
    {
      y = z->parent->parent->left;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if(z == z->parent->left){
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;

  node_t *x, *y;
  y = t->nil;
  x = t->root;
  // 
  while(x != t->nil) {
    y = x;
    if (key < x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  z->parent = y;
  if(y == t->nil)
  {
    t->root = z;
  } 
  else if(key < y->key)
  {
    y->left = z;
  } 
  else
  {
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  insert_fixup(t, z);

  return t->root;
}

// 트리 원소 찾기 -------------------------------------------------------------------------------------

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *find;
  node_t *nil;
  nil = t->nil;
  find = t->root;
  if(find == nil){
    return NULL;
  } else {
    while(find != nil) {
      if(find->key == key)
      {
        return find;

      } else if(find->key > key)
      {
        find = find->left;
      } else {
        find = find->right;
      }
    }
    return NULL;
  }
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *find_min;
  node_t *nil;
  find_min = t->root;
  nil = t->nil;
  if(find_min == nil){
    return NULL;
  } else {

    while(find_min->left != nil) {
      find_min = find_min->left;
    }
    return find_min;
  }
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *find_max;
  node_t *nil;
  find_max = t->root;
  nil = t->nil;
  if(find_max == nil){
    return NULL;
  } else {

    while(find_max->right != nil) {
      find_max = find_max->right;
    }
    return find_max;
  }
}

// 트리 원소 삭제 -------------------------------------------------------------------------------------

node_t *min_subtree(rbtree *t, node_t *n) {
	//printf("min_subtree 진입!!!!\n");
	while (n->left != t->nil)  //여기
	{
		n = n->left;
	}

	return n;
}

node_t *max_subtree(rbtree *t, node_t *n) {
	//printf("max_subtree 진입!!!!\n");
	while (n->right != t->nil)  //여기
	{
		n = n->right;
	}

	return n;
}

void rb_delete_fixup(rbtree *t, node_t *x) {
  node_t *w;
  while(x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {

      w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }

    } else {

      w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }

    }
  }
  x->color = RBTREE_BLACK;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v) {   // 변수 바꾸기!!!
  if(u->parent == t->nil) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *x, *y;
  rbtree *new = (rbtree *)calloc(1, sizeof(rbtree));
  new->nil = t->nil;
  y = p;

  color_t y_original_color = y->color;
  if (p->left == t->nil) {
    x = p->right;
    rb_transplant(t, p, p->right);
  } else if (p->right == t->nil) {
    x = p->left;
    rb_transplant(t, p, p->left);
  } else {
    new->root = p->right;
    y = rbtree_min(new);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == p) {
      x->parent = y;
    } else { 
      rb_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rb_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if(y_original_color == RBTREE_BLACK) {
    rb_delete_fixup(t, x);
  }
  free(new);
  free(p);
  return 0;
}

// 트리 배열 확인 -------------------------------------------------------------------------------------

int inorder_tree_walk(node_t *x, node_t *y, key_t *arr, int *count, const size_t *n) {

  if(x->left != y) 
  { 
    inorder_tree_walk(x->left, y, arr, count, n);
  }
  
  arr[*count] = x->key;
  (*count)++;

  if(*count == (*n) + 1) 
  {
    return 1;
  }

  if(x->right != y)
  {
    inorder_tree_walk(x->right, y, arr, count, n);
  }

  return 0;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) 
{
  int c = 0;
  int *count = &c;

  node_t *x = t->root;
  node_t *y = t->nil;
  if(x == t->nil) 
  {
    return 0;
  } 
  else 
  {
    inorder_tree_walk(x, y, arr, count, &n);
    return 1;
  }
}

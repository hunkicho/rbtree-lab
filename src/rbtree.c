#include "rbtree.h"

#include <stdlib.h>
// 트리 생성 ------------------------------------------------------------------------------------------------

rbtree *new_rbtree(void) {
  rbtree *tree = (rbtree *)calloc(1, sizeof(rbtree));

  // nil 노드 생성
  node_t * nil_n = (node_t*)calloc(1, sizeof(node_t));

  // 초기값으로 tree의 nil과 root를 nil노드로 설정
  nil_n->color = RBTREE_BLACK;
  tree->nil = nil_n;
  tree->root = nil_n;

  return tree;
}

// 트리,노드 메모리 삭제 -------------------------------------------------------------------------------------

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
// rotation 함수 ---------------------------------------------------------------------------------------

static void left_rotation(rbtree* t, node_t* x){
  // 왼쪽 회전이니깐 오른쪽 자식을 포인터 변수에 담는다.
  node_t* y = x->right;

  x->right = y->left; // 원래 있던 x의 오른쪽 자식에 y의 왼쪽 자식 파양

  // y의 왼쪽 자식이 있다면 부모를 x로 변경
  if (y->left != t->nil){
    y->left->parent = x;
  }

  y->parent = x->parent; // y의 부모로 x의 부모로 변경

  if (x->parent == t->nil){ // x의 부모가 없다면 x가 루트였다는 소리니깐 root값에 y대입
    t->root = y;
  }else if (x == x->parent->left){ // x가 부모의 어느 자식이었는지 확인후 그자리에 y대입
    x->parent->left = y;
  }else{
    x->parent->right = y;
  }
  // y의 왼쪽에 x를 넣고 x의 부모를 y로 설정
  y->left = x;
  x->parent = y;
}

static void right_rotation(rbtree* t, node_t* y){ // left와 반대로 하면 된다.
  node_t* x = y->left;
  y->left = x->right;

  if (x->right != t->nil){
    x->right->parent = y;
  }

  x->parent = y->parent;

  if (y->parent == t->nil){
    t->root = x;
  }else if (y == y->parent->left){
    y->parent->left = x;
  }else{
    y->parent->right = x;
  }
  
  x->right = y;
  y->parent = x;
}

// 트리 원소 추가 fix 함수 ----------------------------------------------------------------------------

static void rb_insert_fix(rbtree *t, node_t* z){

  node_t * y; // 삼촌 노드의 포인터를 받을 포인터 변수

  while (z->parent->color == RBTREE_RED){ // 부모가 레드가 아닐때 까지

    if (z->parent == z->parent->parent->left){ // z가 z부모의 왼쪽 자식일 경우

      y = z->parent->parent->right; // y에 z삼촌을 대입

      if (y->color == RBTREE_RED){ // y가 red면 recolor해준다.
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else if (z == z->parent->right){ // y가 black이면 로테이션 함수를 통해 트리 재정립
        z = z->parent;
        left_rotation(t,z);
      }
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      right_rotation(t,z->parent->parent);
    }
    else{ // z가 z부모의 오른쪽 자식일 경우 위와 반대
      y = z->parent->parent->left;

      if (y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else if (z == z->parent->left){
        z = z->parent;
        right_rotation(t, z);
      }
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      left_rotation(t, z->parent->parent);
    }
  }
  t->root->color = RBTREE_BLACK; // 마지막에 혹시 root의 색깔이 red로 변했을수도 있으니 다시 black으로 칠한다.
}


// 트리 원소 추가 -------------------------------------------------------------------------------------

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // key값을 가진 새로운 노드 z 생성
  node_t * z = (node_t*)calloc(1,sizeof(node_t));
  z->key = key;

  // 새로운 노드의 자리를 잡기 위해 필요한 포인터들 초기값 세팅
  node_t* x = t->root;
  node_t* y = t->nil;

  // 새로운 값이 들어갈 자리를 while문으로 찾는다.
  while (x != t->nil){
    y = x;
    if (z->key < x->key){
      x = x->left;
    }else{
      x = x->right;
    }
  }

  z->parent = y; // 들어갈 자리를 찾았다면 z의 부모로 y를 대입

  // y의 조건에 따라 z위치 정립
  if (y == t->nil){
    t->root = z;
  }else if (z->key < y->key){
    y->left = z;
  }else{
    y->right = z;
  }
  // 자리를 잡았다면 이제 z의 세부정보 세팅
  z->color = RBTREE_RED;
  z->left = t->nil;
  z->right = t->nil;

  // z로 인해 생길수 있는 rb트리 조건의 위법이 있나 확인후 있으면 처리하기 위해 fix함수 호출
  rb_insert_fix(t, z);

  return z;
}

// 트리 원소 찾기 -------------------------------------------------------------------------------------

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // 구성 요소가 없을 경우
  if (t->root == t->nil){
    return NULL;
  }
  // find를 하기위해 만든 포인터 변수
  node_t* find = t->root;
  
  while (find != t->nil){
    if (find->key == key){ // 찾는 값을 찾았을 경우 포인터 리턴
      return find;
    }
    if (find->key < key){
      find = find->right; // 현재값보다 찾는값이 크다면 오른쪽 자식으로 이동
    }else{
      find = find->left; // 현재값보다 찾는값이 작다면 왼쪽 자식으로 이동
    };
  }
  return NULL;
}

// 원소 최소값 찾기 -------------------------------------------------------------------------------------

node_t *rbtree_min(const rbtree *t) {
  // 트리를 구성하는 원소가 하나도 없을 경우
  if (t->root == t->nil){
    return NULL;
  }
  // 최소값을 확인하기 위해 만든 min포인터
  node_t *min = t->root;
  // min의 왼쪽 자식이 nil값이 나올때 까지 진행
  while (min->left != t->nil){
    min = min->left;
  }
  return min;
}

// 원소 최대값 찾기 -------------------------------------------------------------------------------------

node_t *rbtree_max(const rbtree *t) {
  // 트리를 구성하는 원소가 없을 경우
  if (t->root == t->nil){
    return NULL;
  }
  // 최대값 확인을 위한 노드 포인터 생성
  node_t* max = t->root;
  // 포인터의 오른쪽 자식이 nil값이 나올때까지
  while (max->right != t->nil){
    max = max->right;
  }
  
  return max;
}

// 서브 트리의 min,max / transplant -------------------------------------------------------------------

node_t* sub_min(rbtree* t, node_t* node){

  node_t* sub_min_node = node; // 서브 트리의 min값을 담을 노드 포인터

  while (sub_min_node->left != t->nil){
    sub_min_node = sub_min_node->left;
  }

  return sub_min_node;
}

node_t* sub_max(rbtree* t, node_t* node){

  node_t* sub_max_node = node;

  while (sub_max_node->right != t->nil){
    sub_max_node = sub_max_node->right;
  }

  return sub_max_node;
}

static void transplant(rbtree* t, node_t* u, node_t* v){
  // u의 위치에 따라 v의 세부 정보를 세팅
  if (u->parent == t->nil){
    t->root = v;
  }else if (u == u->parent->left){
    u->parent->left = v;
  }else{
    u->parent->right = v;
  }
  // v부모에 u부모 대입
  v->parent = u->parent;
}

// 트리 원소 삭제 -------------------------------------------------------------------------------------

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

// 트리 배열 확인 -------------------------------------------------------------------------------------

void ino(const rbtree* t, node_t* node, key_t *arr, const size_t n, int *count){
  if (node == t->nil){
    return;
  }

  ino(t, node->left, arr, n, count);
  if (*count < n){
    arr[*count] = node->key;
    (*count)++;
  }else{
    return;
  }
  ino(t, node->right, arr, n, count);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // 빈 트리
  if (t->root == t->nil){
    return 0;
  }

  int count = 0;
  ino(t, t->root, arr, n, &count);

  // 배열이 다 채워졌는지 확인
  for (int i = 0; i < n; i++){ 
    if (arr[i] == 0){
      return 0;
    }
  }
  return 1;
}

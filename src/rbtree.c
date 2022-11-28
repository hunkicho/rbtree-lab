#include "rbtree.h"

#include <stdlib.h>

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
  }else{ 
    free(t->root); // 빈 트리라면 root와 nil 그리고 트리 t를 차례로 지워준다.
  }
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

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

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

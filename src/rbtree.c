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

// 트리 원소 추가 -------------------------------------------------------------------------------------

//알고리즘 책에 나온 회전
void left_rotation(rbtree* t, node_t* x)
{
	//printf("left_rotation 진입!!!!\n");
	node_t* y;

	y = x->right;                   // y를 설정한다.
	x->right = y->left;             // y의 왼쪽 서브 트리를 x의 오른쪽 서브 트리로 옮긴다.

	if (y->left != t->nil)
	{
		y->left->parent = x;
	}
	y->parent = x->parent;          // x의 부모를 y로 연결한다.

	if (x->parent == t->nil)
	{
		t->root = y;
	}
	else if (x == x->parent->left)
	{
		x->parent->left = y;
	}
	else
	{
		x->parent->right = y;
	}
	y->left = x;                    // x를 y의 왼쪽으로 놓는다
	x->parent = y;
}

// 알고리즘 책에 나온 회전(left_rotation과 대칭)
void right_rotation(rbtree* t, node_t* y)
{
	//printf("right_rotation 진입!!!!\n");
	node_t* x;

	x = y->left;
	y->left = x->right;
	if (x->right != t->nil)
	{
		x->right->parent = y;
	}
	x->parent = y->parent;

	if (y->parent == t->nil)
	{
		t->root = x;
	}
	else if (y == y->parent->right)
	{
		y->parent->right = x;
	}
	else
	{
		y->parent->left = x;
	}
	x->right = y;
	y->parent = x;
}

//새로운 노드를 red로 설정 시 rb tree 특성 위배될 수 있기 때문에 수정
void rb_insert_fixup(rbtree *t, node_t *z) {
	//printf("rb_insert_fixup 진입!!!!\n");

	//node_t* y = (node_t *)malloc(sizeof(node_t));
	node_t* y;


	while (z->parent->color == RBTREE_RED)             // z의 부모가 red이면 조상도 반드시 존재(rb트리의 루트는 black이기 때문에)
	{
		if (z->parent == z->parent->parent->left)       //삽입된 원소의 부모가 조상의 leftchild면
		{
			y = z->parent->parent->right;               //y설정 - 삽입된 원소의 조상의 rightchild(uncle)

			if (y->color == RBTREE_RED)                 //uncle이 red면
			{
				z->parent->color = RBTREE_BLACK;        //부모도 검은색
				y->color = RBTREE_BLACK;                //uncle도 검은색
				z->parent->parent->color = RBTREE_RED;  //조상을 red
				z = z->parent->parent;                  //위치를 조상으로 옮김
			}
			else                                        // uncle이 black이면
			{
				if (z == z->parent->right)              //내가 부모의 rightchild면
				{
					z = z->parent;                      // 위치를 부모로 옮김
					left_rotation(t, z);
				}
				z->parent->color = RBTREE_BLACK;        //부모를 black으로
				z->parent->parent->color = RBTREE_RED;  //조상을 red로
				right_rotation(t, z->parent->parent);
			}
		}
		else                                           //삽입된 원소의 부모가 조상의 rightchild면
		{
			y = z->parent->parent->left;               //y설정 - 삽입된 원소의 조상의 leftchild(uncle)

			if (y->color == RBTREE_RED)                //uncle이 red면
			{
				z->parent->color = RBTREE_BLACK;       //부모도 검은색
				y->color = RBTREE_BLACK;               //uncle도 검은색
				z->parent->parent->color = RBTREE_RED; //조상을 red
				z = z->parent->parent;                 //위치를 조상으로 옮김
			}
			else
			{
				if (z == z->parent->left)              //내가 부모의 leftchild면
				{
					z = z->parent;                     // 위치를 부모로 옮김
					right_rotation(t, z);
				}
				z->parent->color = RBTREE_BLACK;       //부모를 black으로
				z->parent->parent->color = RBTREE_RED; //조상을 red
				left_rotation(t, z->parent->parent);
			}
		}
	}
	t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  	node_t* z = (node_t *)malloc(sizeof(node_t));      //새로 추가할 노드 설정
	node_t* x = t->root;
	node_t* y = t->nil;

	z->key = key;           // 포인터에 키값 주기

	while (x != t->nil)     // 닐노드 안나올 때 까지
	{
		y = x;                      // y가 x의 부모가 되도록(삽입할 위치를 설정하기 위해)
		if (z->key < x->key)        // 삽입할 값이 현재보다 작으면 레프트, 크면 라이트(트리특성)
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}
	z->parent = y;                 //위에서 y에 현재값 넣었

	if (y == t->nil)
	{
		t->root = z;
	}
	else if (z->key < y->key)
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
	rb_insert_fixup(t, z);
	
  return t->root;
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
        left_rotation(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotation(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotation(t, x->parent);
        x = t->root;
      }

    } else {

      w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotation(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotation(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotation(t, x->parent);
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

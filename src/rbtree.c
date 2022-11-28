#include "rbtree.h"
#include<stdio.h>
#include<stdlib.h>

//rbtree 생성
rbtree *new_rbtree(void) {
	//printf("new_rbtree 진입!!!!\n");
	rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
	// TODO: initialize struct if needed
	node_t* nil = (node_t *)malloc(sizeof(node_t));

	nil->color = RBTREE_BLACK;
	nil->key = -1;

	p->nil = nil;
	p->root = nil;

	return p;
}

//rbtree 지우기
void delete_rbtree(rbtree *t) {
	//printf("delete_rbtree 진입!!!!\n");
	// TODO: reclaim the tree nodes's memory
	free(t);
}

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


	while (z->parent->color == RBTREE_RED)
	{
		if (z->parent == z->parent->parent->left)
		{
			y = z->parent->parent->right;

			if (y->color == RBTREE_RED)
			{
				z->parent->color = RBTREE_BLACK;
				y->color = RBTREE_BLACK;
				z->parent->parent->color = RBTREE_RED;
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->right)
				{
					z = z->parent;
					left_rotation(t, z);
				}
				z->parent->color = RBTREE_BLACK;
				z->parent->parent->color = RBTREE_RED;
				right_rotation(t, z->parent->parent);
			}
		}
		else
		{
			y = z->parent->parent->left;

			if (y->color == RBTREE_RED)
			{
				z->parent->color = RBTREE_BLACK;
				y->color = RBTREE_BLACK;
				z->parent->parent->color = RBTREE_RED;
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->left)
				{
					z = z->parent;
					right_rotation(t, z);
				}
				z->parent->color = RBTREE_BLACK;
				z->parent->parent->color = RBTREE_RED;
				left_rotation(t, z->parent->parent);
			}
		}
	}
	t->root->color = RBTREE_BLACK;
}

//rb tree에 node 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
	//printf("rbtree_insert 진입!!!!\n");
	// TODO: implement inserts
	node_t* z = (node_t *)malloc(sizeof(node_t));      //새로 추가할 노드 설정
	node_t* x = t->root;
	node_t* y = t->nil;

	z->key = key;

	while (x != t->nil)
	{
		y = x;
		if (z->key < x->key)
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}
	z->parent = y;

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

//요소 찾기
node_t *rbtree_find(const rbtree *t, const key_t key) {
	//printf("rbtree_find 진입!!!!\n");
	// TODO: implement find
	// rb tree에서 left child는 parent보다 작고, right child는 parent보다 크다.
	node_t* first = t->root;
	//printf("first %d\n", first->key);
	while (first->key != -1)
	{
		if (key == first->key)
		{
			return first;
		}
		else if (key > first->key)
		{
			first = first->right;
		}
		else if (key < first->key)
		{
			first = first->left;
		}
	}
	return NULL;

}

//노드 중 최솟값 찾기
node_t *rbtree_min(const rbtree *t) {
	//printf("rbtree_min 진입!!!!\n");
	// TODO: implement find
	node_t* first = t->root;

	// rb tree에서는 left child는 parent보다 작고 right child는 parent보다 크기 때문에 root에서 nil까지 계속 left 호출
	while (first->key != -1)
	{
		if (first->left->key == -1) {
			break;
		}
		first = first->left;
	}
	return first;
}

node_t *rbtree_max(const rbtree *t) {
	//printf("rbtree_max 진입!!!!\n");
	// TODO: implement find
	node_t* first = t->root;

	// rb tree에서는 left child는 parent보다 작고 right child는 parent보다 크기 때문에 root에서 nil까지 계속 right 호출
	while (first->key != -1)
	{
		if (first->right->key == -1) {
			break;
		}
		first = first->right;
	}
	return first;
}

node_t *min_subtree(node_t *n) {
	//printf("min_subtree 진입!!!!\n");
	while (n->left->key != -1)
	{
		if (n->left->key == -1)
		{
			break;
		}
		n = n->left;
	}

	return n;
}

node_t *max_subtree(node_t *n) {
	//printf("max_subtree 진입!!!!\n");
	while (n->right->key != -1)
	{
		if (n->right->key == -1)
		{
			break;
		}
		n = n->right;
	}

	return n;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
	//printf("rbtree_transplant 진입!!!!\n");
	if (u->parent == t->nil)
	{
		t->root = v;
	}
	else if (u == u->parent->left)
	{
		u->parent->left = v;
	}
	else
	{
		u->parent->right = v;
	}
	v->parent = u->parent;
}

void rb_erase_fixup(rbtree *t, node_t *x)
{
	//printf("rb_erase_fixup 진입!!!!\n");
	//node_t* w = (node_t *)malloc(sizeof(node_t));
	node_t* w;
	while (x != t->root && x->color == RBTREE_BLACK)
	{
		if (x == x->parent->left)
		{
			w = x->parent->right;

			if (w->color == RBTREE_RED)
			{
				w->color = RBTREE_BLACK;
				x->parent->color = RBTREE_RED;
				left_rotation(t, x->parent);
				w = x->parent->right;
			}

			if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
			{
				w->color = RBTREE_RED;
				x = x->parent;
			}
			else
			{
				if (w->right->color == RBTREE_BLACK)
				{
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
		}
		else
		{
			w = x->parent->left;

			if (w->color == RBTREE_RED)
			{
				w->color = RBTREE_BLACK;
				x->parent->color = RBTREE_RED;
				right_rotation(t, x->parent);
				w = x->parent->left;
			}

			if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
			{
				w->color = RBTREE_RED;
				x = x->parent;
			}
			else
			{
				if (w->left->color == RBTREE_BLACK)
				{
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

int rbtree_erase(rbtree *t, node_t *z) {
	//printf("rbtree_erase 진입!!!!\n");
	// TODO: implement erase
	// node_t* x = (node_t *)malloc(sizeof(node_t));
	// node_t* y = (node_t *)malloc(sizeof(node_t));
	node_t* x;
	node_t* y;

	y = z;
	color_t og_color = y->color;

	if (z->left == t->nil)
	{
		x = z->right;
		rbtree_transplant(t, z, z->right);
	}
	else if (z->right == t->nil)
	{
		x = z->left;
		rbtree_transplant(t, z, z->left);
	}
	else
	{
		y = min_subtree(z->right);
		og_color = y->color;
		x = y->right;
		if (y->parent == z)
		{
			x->parent = y;
		}
		else
		{
			rbtree_transplant(t, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		rbtree_transplant(t, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}

	if (og_color == RBTREE_BLACK)
	{
		rb_erase_fixup(t, x);
	}


	return 0;
}

key_t *inorder(node_t* node, const size_t n, int num) {
	key_t *rtn_arr = malloc(n * sizeof(key_t));

	// if (node->key == -1)
	// {
	// 	printf("닐노드 온듯?\n");
	// 	for (int i = 0; i < n; i++) {
    // 		printf("%d,",rtn_arr[i]);
  	// 	}
	// 	printf("\n");
	// 	return 0;
	// }

	printf("루트 받아왔나? %d\n", node->key);

	if(node->left->key != -1)
	{
		inorder(node->left, n, num);
	}
	
	if(node->key != -1)
	{
		rtn_arr[n] = node->key;
	}

	if(node->right->key != -1)
	{
		inorder(node->right, n, num);
	}

	num++;
	printf("%d 번쨰\n",num);

	if (num > n)
	{
		printf("제대로 왔습니다\n");
		for (int i = 0; i < n; i++) {
    		printf("%d,",rtn_arr[i]);
  		}
		printf("\n");
		return rtn_arr;
	}
	printf("뭔가 오류가 있는듯\n");
	return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
	//printf("rbtree_to_array 진입!!!!\n");
	// TODO: implement to_array

	arr = inorder(t->root, n, 0);

	// for (int i = 0; i < n; i++) {
	// 	node_t * node = rbtree_min(t);
	// 	arr[i] = node->key;
	// 	rbtree_erase((rbtree *)t,rbtree_min(t));
	// }
	return 0;
}

//  int main()
//  {
//  	rbtree *a = new_rbtree();
//  	node_t *n = rbtree_insert(a, 5);
//  	printf("루트는: %d\n", n->key);
//  	n = rbtree_insert(a, 7);
//  	printf("루트는: %d\n", n->key);
//  	n = rbtree_insert(a, 7);
//  	printf("루트는: %d\n", n->key);
//  	n = rbtree_insert(a, 12);
//  	printf("루트는: %d\n", n->key);
//  	n = rbtree_insert(a, 8);
//  	printf("루트는: %d\n", n->key);
//  	n = rbtree_insert(a, 6);
//  	printf("루트는: %d\n", n->key);
//  	n = rbtree_insert(a, 24);
//  	printf("루트는: %d\n", n->key);
//  	n = rbtree_insert(a, 23);
//  	printf("루트는: %d\n", n->key);
//  	n = rbtree_insert(a, 22);
//  	printf("루트는: %d\n", n->key);
//  	n = rbtree_insert(a, 35);
//  	printf("루트는: %d\n", n->key);
//  	n = rbtree_insert(a, 1);
//  	printf("루트는: %d\n", n->key);
//  	n = rbtree_insert(a, 98);
//  	printf("루트는: %d\n", n->key);
//  	printf("루트 leftchild는: %d\n", n->left->key);
//  	node_t * f = rbtree_find(a,22);
//  	printf("인덱스는 %p\n",f);
//  	node_t * m = rbtree_min(a);
//  	printf("최소값은 %d\n", m->key);
//  	node_t * x = rbtree_max(a);
//  	printf("최대값은 %d\n", x->key);


//  	//int b[] = { 10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25 };
//  	//rbtree_to_array(a, b, sizeof(b) / sizeof(int));

// 	system("pause");

//  	delete_rbtree(a);
//  }
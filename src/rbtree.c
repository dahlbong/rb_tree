#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *NIL = (node_t*)calloc(1, sizeof(node_t));
  NIL->color = RBTREE_BLACK;
  p->root = p->nil = NIL;
  return p;
}

// 해당 tree가 사용했던 메모리를 전부 반환해야 합니다. (valgrind로 나타나지 않아야 함)
void delete_rbtree(rbtree *t) {
  delete_rbtree_sub(t, t->root);    //노드들 메모리 전부 해제
  free(t->nil);                     //nil 메모리 해제 (root는 노드 메모리 해제할 때 기수행)
  free(t);                          //구조체 메모리 해제
}

void delete_rbtree_sub(rbtree *t, node_t *p) {
  if (p != t->nil) {
    //재귀적으로 메모리 해제 수행
    delete_rbtree_sub(t, p->left);    
    delete_rbtree_sub(t, p->right);
    free(p); 
  }
}

// 구현하는 ADT가 multiset이므로 이미 같은 key의 값이 존재해도 하나 더 추가 합니다.
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *x = t->root;                                  //key의 비교 대상 노드
  node_t *y = t->nil;                                   //key의 부모 노드
  node_t *cur = (node_t*)calloc(1, sizeof(node_t));     //삽입 노드 메모리 할당해주기

  //root부터 아래로 노드 삽입 위치 찾아가기
  while (x != t->nil) {
    y = x;
    if (x->key > key) x = x->left;
    else x = x->right;
  }
  
  // 위치 찾았으니 cur의 정보(key, color, parent, left, right) 초기화
  cur->key = key;
  cur->parent = y;

  // cur 위치에 따라 부모의 자식노드 업데이트 해주기
  if (y == t->nil) t->root = cur;
  else if (cur->key < y->key) y->left = cur;
  else y->right = cur;
  cur->color = RBTREE_RED;
  cur->left = cur->right = t->nil;
  rbtree_insert_fixup(t, cur);

  return t->root;     //속성유지
}

// 불균형 복구
void rbtree_insert_fixup(rbtree *t, node_t *z) {

  node_t *y;

  // 부모노드가 BLACK 될때까지 반복
  while (z->parent->color == RBTREE_RED) {
    // 부모노드가 조부모의 왼쪽 자식일 때
    if (z->parent == z->parent->parent->left) {
      y = z->parent->parent->right;

      // case 1: 삽입노드의 삼촌 노드가 RED  => 부모 레벨의 색과 조부모의 색 스왑
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // case 2: 삼촌노드가 BLACK이고 삽입노드가 오른쪽 자식일 때 => 회전
      else {
        if (z == z->parent->right) {
          z = z->parent;
          rotate_left(t, z);
        }
        // case 3: 삼촌노드가 BLACK이고 삽입노드가 왼쪽 자식일 때 => 부모와 조부모 색 스왑 후 회전
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rotate_right(t, z->parent->parent);
      }
    }

    // 부모노드가 조부모의 왼쪽 자식일 때
    else {
      y = z->parent->parent->left;
      // case 1: 삽입노드의 삼촌 노드가 RED  => 부모 레벨의 색과 조부모의 색 스왑
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // case 2: 삼촌노드가 BLACK이고 삽입노드가 오른쪽 자식일 때 => 회전
      else {
        if (z == z->parent->left) {
          z = z->parent;
          rotate_right(t, z);
        }
        // case 3: 삼촌노드가 BLACK이고 삽입노드가 왼쪽 자식일 때 => 부모와 조부모 색 스왑 후 회전
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rotate_left(t, z->parent->parent);        
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

void rotate_left(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;                                  //y의 왼쪽 서브트리를 x의 오른쪽 서브트리로 회전
  if (y->left != t->nil) y->left->parent = x;  //y이 왼쪽 자식노드를 가지고 있다면 x의 오른쪽 자식으로 바꿔주기
  y->parent = x->parent;                               //y의 부모노드 업데이트

  // x이 root인 경우
  if (x->parent == t->nil) t->root = y;
  // x이 왼쪽 자식인 경우
  else if (x == x->parent->left) x->parent->left = y;
  // x이 오른쪽 자식인 경우
  else x->parent->right = y;

  // y의 왼쪽 자식과 x의 부모 노드 업데이트
  y->left = x;
  x->parent = y;
}

void rotate_right(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;                                      //y의 오른쪽 서브트리를 x의 왼쪽 서브트리로 회전
  if (y->right != t->nil) y->right->parent = x;    //y이 오른쪽 자식노드를 가지고 있다면 x의 왼쪽 자식으로 바꿔주기
  y->parent = x->parent;                                   //y의 부모노드 업데이트

  if(x->parent == t->nil) t->root = y;
  else if (x == x->parent->right) x->parent->right = y;
  else x->parent->left = y;

  y->right = x;
  x->parent = y;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // RB tree내에 해당 key가 있는지 탐색하여 있으면 해당 node pointer 반환, 없으면 NULL 반환
  node_t * cur = t->root;

  while (cur != t->nil) {
    if (cur->key == key) return cur;
    if (cur->key < key) cur = cur->right;
    else cur = cur->left;
  }
  return NULL;
}

// 노드 중 가장 작은 키 값 반환
node_t *rbtree_min(const rbtree *t) {
  if (t->root == t->nil) return NULL;
  node_t *cur = t->root;
  while (cur->left != t->nil) cur = cur->left;
  
  return cur;
}

// 노드 중 가장 큰 키 값 반환
node_t *rbtree_max(const rbtree *t) {
  if (t->root == t->nil) return NULL;
  node_t *cur = t->root;
  while (cur->right != t->nil) cur = cur->right;

  return cur;
}

// 노드 삭제
int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y = p;
  node_t *x;
  color_t y_original_color = y->color;

  if (p->left == t->nil) {
    x = p->right;
    rbtree_transplant(t, p, p->right);
  } else if (p->right == t->nil) {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  } else {
    y = tree_minimum(t, p);
    y_original_color = y->color;
    x = y->right;
    if (y != p->right) {
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    } else {
      x->parent = y;
      }
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
    }
  if (y_original_color == RBTREE_BLACK) {
  rbtree_erase_fixup(t, x);
  }
  free(p);
  return 0;
}

// son을 del 자리로 옮기는 함수
void rbtree_transplant(rbtree *t, node_t *del, node_t *son) {
  if (del->parent == t->nil) t->root = son;                     //root일 경우
  else if (del == del->parent->left) del->parent->left = son;   //왼쪽 자식일 경우
  else del->parent->right = son;                                //오른쪽 자식일 경우
  son->parent = del->parent;
}

// 후임자(삭제 노드보다 큰 노드 중 가장 작은 노드) 찾는 함수
node_t *tree_minimum(rbtree *t, node_t *node) {
  node_t *cur = node->right;
  node_t *min = cur;

  // 후임자가 없을 경우 NULL 반환
  if(cur == t->nil) return NULL;

  while(cur != t->nil) {
    min = cur;
    cur = cur->left;
  }
  return min;
}

void rbtree_erase_fixup(rbtree *t, node_t *p) {
  while (p != t->root && p->color == RBTREE_BLACK) {
    if (p == p->parent->left) {
      node_t *w = p->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        rotate_left(t, p->parent);
        w = p->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        p = p->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_right(t, w);
          w = p->parent->right;
        }
        w->color = p->parent->color;
        p->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        rotate_left(t, p->parent);
        p = t->root;
      }
    } else {
      node_t *w = p->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        rotate_right(t, p->parent);
        w = p->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        p = p->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_left(t, w);
          w = p->parent->left;
        }
        w->color = p->parent->color;
        p->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rotate_right(t, p->parent);
        p = t->root;
      }
    }
  }
  p->color = RBTREE_BLACK;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // RB tree의 내용을 key 순서대로 주어진 array로 변환
  if (t == NULL || arr == NULL || n == 0) return 0;
  
  size_t index = 0;
  rbtree_to_array_recursive(t, t->root, arr, n, &index);
  
  return index;
}

void rbtree_to_array_recursive(const rbtree *t, const node_t *node, key_t *arr, const size_t n, size_t *index) {
  if (node == t->nil || *index >= n) return;
  
  rbtree_to_array_recursive(t, node->left, arr, n, index);
  
  if (*index < n) {
    arr[*index] = node->key;
    (*index)++;
  }
  
  rbtree_to_array_recursive(t, node->right, arr, n, index);
}
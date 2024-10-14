#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t*)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->nil = p->root = nil;   //트리가 빈 경우 root가 nil 노드이므로
  return p;
}

void delete_rbtree(rbtree *t) {
  // 해당 tree가 사용했던 메모리를 전부 반환해야 합니다. (valgrind로 나타나지 않아야 함)
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

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // 구현하는 ADT가 multiset이므로 이미 같은 key의 값이 존재해도 하나 더 추가 합니다.


  return t->root;
}

void left_rotate(rbtree *t, node_t *curTop) {
  node_t *targetTop = curTop->right;
  curTop->right = targetTop->left;                                  //targetTop의 왼쪽 서브트리를 curTop의 오른쪽 서브트리로 회전
  if (targetTop->left != t->nil) targetTop->left->parent = curTop;  //targetTop이 왼쪽 자식노드를 가지고 있다면 curTop의 오른쪽 자식으로 바꿔주기
  targetTop->parent = curTop->parent;                               //targetTop의 부모노드 업데이트

  // curTop이 root인 경우
  if (curTop->parent == NULL) t->root = targetTop;
  // curTop이 왼쪽 자식인 경우
  else if (curTop == curTop->parent->left) curTop->parent->left = targetTop;
  // curTop이 오른쪽 자식인 경우
  else curTop->parent->right = targetTop;

  // targetTop의 왼쪽 자식과 curTop의 부모 노드 업데이트
  targetTop->left = curTop;
  curTop->parent = targetTop;
}

void right_rotate(rbtree *t, node_t *curTop) {
  node_t *targetTop = curTop->right;
  curTop->left = targetTop->right;                                      //targetTop의 오른쪽 서브트리를 curTop의 왼쪽 서브트리로 회전
  if (targetTop->right != t->nil) targetTop->right->parent = curTop;    //targetTop이 오른쪽 자식노드를 가지고 있다면 curTop의 왼쪽 자식으로 바꿔주기
  targetTop->parent = curTop->parent;                                   //targetTop의 부모노드 업데이트
  
  if(curTop->parent == t->nil) t->root = targetTop;
  else if (curTop == curTop->parent->right) curTop->parent->right = targetTop;
  else curTop->parent->left = targetTop;

  targetTop->right = curTop;
  curTop->parent = targetTop;
}

void *rbtree_insert_fixup(rbtree *t, const key_t key) {
  //불균형 복구

}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // RB tree내에 해당 key가 있는지 탐색하여 있으면 해당 node pointer 반환, 없으면 NULL 반환

  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // 노드 중 가장 작은 키 값 반환
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // 노드 중 가장 큰 키 값 반환
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // 노드 삭제
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // RB tree의 내용을 key 순서대로 주어진 array로 변환
  return 0;
}

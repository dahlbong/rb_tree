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

void *rbtree_insert_fixup(rbtree *t, const key_t key) {

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

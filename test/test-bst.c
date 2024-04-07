#include <assert.h>
#include <bst.h>
#include <stdio.h>

void test_init(const int key) {
  TreeNode *root = createTreeNode(key);
  assert(root != NULL);
  freeTree(root);
}

void test_insert(const int key) {
  TreeNode *root = createTreeNode(1);
  insert(root, key);
  TreeNode *child = search(root, key); 
  assert(child != NULL);
  if (root -> key <= key){
    assert(root -> right == child);  
  }else{
    assert(root -> left == child);
  }
  assert(child->key == key);
  freeTree(root);
}

// find should return the node with the key or NULL if no such node exists
void test_search(const int key, const int wrong_key) {
  TreeNode *root = createTreeNode(5);
  insert(root, 1);
  insert(root, 2);
  insert(root, key);
  insert(root, 4);

  TreeNode *searchNode = search(root, key);
  assert(searchNode != NULL);
  assert(searchNode->key == key);

  TreeNode *wrongSearchNode = search(root, wrong_key);
  assert(wrongSearchNode == NULL);

  freeTree(root);
}

void test_inorderTraversal(const int key) {
  TreeNode *root = createTreeNode(key);
  for(int i=1; i<=10; i++){
      insert(root, i);
  }
  inorderTraversal(root);
  printf("\n");
  freeTree(root);
}

void test_delete(const int key){
  TreeNode *root = createTreeNode(10);
  insert(root, 5);
  insert(root, 3);
  insert(root, 1);
  insert(root, 4);

  insert(root, 15);
  insert(root, 20);
  insert(root, 18);
  insert(root, 17);
  insert(root, 19);
  insert(root, 25);
  insert(root, 24);
  insert(root, 26);

  inorderTraversal(root);
  printf("\n");
  deleteNode(root, key);
  inorderTraversal(root);
  printf("\n");
}



int main(void) {
  test_init(1);
  test_insert(2);
  test_search(3, 6);
  test_inorderTraversal(5);

  // 양쪽 노드 모두 없는 경우
  test_delete(17);
  // 오른쪽 노드가 있는 경우
  test_delete(15);
  // 왼쪽 노드가 있는 경우
  test_delete(5);
  // 양쪽 노드 모두 있는 경우
  test_delete(20);
  printf("Passed all tests!\n");
}

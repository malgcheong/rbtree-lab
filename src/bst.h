#ifndef _BST_H_
#define _BST_T_

typedef struct TreeNode{
    int key;
    struct TreeNode *left;
    struct TreeNode *right;

} TreeNode;

// 새로운 이진 트리 노드 생성
TreeNode* createTreeNode(int key);

// 이진 트리에 키 삽입
void insert(TreeNode* root, int key);

// 이진 트리에서 키 검색
TreeNode* search(TreeNode* root, int key);

// 이진 트리에서 키 삭제
void deleteNode(TreeNode* root, int key);

// 이진 트리 출력 (중위 순회)
void inorderTraversal(TreeNode* root);

// 이진 트리 메모리 해제
void freeTree(TreeNode* root);

#endif // _BST_H_

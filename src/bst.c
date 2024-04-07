#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

// 새로운 이진 트리 노드 생성
TreeNode* createTreeNode(int key){
    TreeNode *root = (TreeNode *)malloc(sizeof(TreeNode));
    (*root).key = key;
    (*root).left = NULL;
    (*root).right = NULL;
    return root;
}

// 이진 트리에 키 삽입
void insert(TreeNode* root, int key){
    
    TreeNode *temp = root;
    TreeNode *prev = NULL; // 이전 노드를 추적하여 새 노드를 연결할 때 사용

    while (temp){
        prev = temp;
        if (key >= temp -> key){
            temp = temp -> right;
        } else if (key < temp -> key){
            temp = temp -> left;
        }
    }
    TreeNode *newNode = createTreeNode(key);

    // 트리가 비어있는 경우 바로 루트노르로 반환
    if (prev == NULL) {
        return;
    }

    // 이전 노드의 자식 포인터를 새 노드로 설정
    if (key >= prev->key) {
        prev->right = newNode;
    } else {
        prev->left = newNode;
    }

    return;
}

// 이진 트리에서 키 검색
TreeNode* search(TreeNode* root, int key){
    TreeNode *temp = root;
    while (temp){
        if (key == temp -> key){
            return temp;
        } else if (key > temp -> key){
            temp = temp -> right;
        } else if (key < temp ->key){
            temp = temp -> left;
        }
    }
    return NULL;
}

// 이진 트리에서 키 삭제
void deleteNode(TreeNode* root, int key){
    TreeNode* temp = root;
    TreeNode* prev = NULL;
    while (temp){
        if (key == temp -> key){
            break;
        }
        if (key > temp -> key){
            prev = temp;
            temp = temp -> right;
        } else if (key < temp -> key){
            prev = temp;
            temp = temp -> left;
        }
    }
    
    if (temp == NULL){
        printf("Unable to find key\n");
        return;
    }

    // 자식 노드가 없는 경우는 0
    // 한쪽 노드만 있는 경우는 1
    // 자식 노두 모두 있는 경우는 2
    int count = 0;
    int right = 0;
    if (temp -> left != NULL){
        count += 1;
    }
    if (temp -> right != NULL){
        count += 1;
        right = 1;
    }

    // temp가 prev의 오른쪽인지 왼쪽인지 판별
    char prevDirection;
    if (prev -> right -> key == key){
        prevDirection = 'R';
    } else if (prev -> left -> key == key){
        prevDirection = 'L';
    }

    switch (count){
        // 자식 노드가 없는 경우
        case 0:
            if (prevDirection == 'R'){
                free(prev->right);
                prev -> right = NULL;
            } else if (prevDirection == 'L'){
                free(prev->left);
                prev -> left = NULL;
            }
            break;
        // 한쪽 노드만 있는 경우
        case 1:
            // 왼쪽 노드만 있는 경우
            if (right != 1){
                TreeNode* deep = temp -> left;
                TreeNode* prevDeep = temp;
                while (deep){
                    if (deep -> right != NULL){
                        prevDeep = deep;
                        deep = deep -> right;
                    } else{
                        break;
                    }
                }

                if (deep == temp -> left){
                    // 1. 삭제할 놈 왼쪽 노드를 삭제할 놈 부모 왼쪽 노드로 연결
                    prev -> left = temp -> left;

                    // 2. 삭제할려던 놈 삭제
                    free(temp); 
                } else {
                    // 1 그나마 큰놈 연결하던 부모노드의 오른쪽 포인터 NULL
                    prevDeep -> right = NULL;

                    // 2. 그나마 큰놈을 삭제할려던 놈 부모 노드와 연결
                    prev -> left = deep;

                    // 3. 그나마 큰 놈의 왼쪽 자식노드로 temp의 왼쪽 노드를 연결
                    deep -> left = temp -> left;

                    // 4. 삭제할려던 놈 삭제
                    free(temp);
                }

                break;
            // 오른쪽 노드만 있는 경우
            } else{
                TreeNode* deep = temp -> right;
                TreeNode* prevDeep = temp;
                while (deep){
                    if (deep -> left != NULL){
                        prevDeep = deep;
                        deep = deep -> left;
                    } else{
                        break;
                    }
                }

                if (deep == temp -> right){
                    // 1. 삭제할 놈 오른쪽 노드를 삭제할 놈 부모 오른쪽 노드로 연결
                    prev -> right = temp -> right;

                    // 2. 삭제할려던 놈 삭제
                    free(temp); 
                } else {
                    // 1 그나마 큰놈 연결하던 부모노드의 왼쪽 포인터 NULL
                    prevDeep -> left = NULL;

                    // 2. 그나마 큰놈을 삭제할려던 놈 부모 노드와 연결
                    prev -> right = deep;

                    // 3. 그나마 큰 놈의 오른쪽 자식노드로 temp의 오른쪽 노드를 연결
                    deep -> right = temp -> right;

                    // 4. 삭제할려던 놈 삭제
                    free(temp);
                }
                break;
            }

        // 양쪽 노드 모두 있는 경우
        case 2:
            TreeNode* deep = temp -> right;
            TreeNode* prevDeep = temp;
            while (deep){
                if (deep -> left != NULL){
                    prevDeep = deep;
                    deep = deep -> left;
                } else{
                    break;
                }
            }
            if (deep == temp -> right){
                // 1. 삭제할 놈 오른쪽 노드를 삭제할 놈 부모 오른쪽 노드로 연결
                prev -> right = temp -> right;

                // 2. 삭제할려던 놈 삭제
                free(temp); 
            } else {
                
                // 1 그나마 큰놈 연결하던 부모노드의 오른쪽 포인터 NULL
                prevDeep -> left = NULL;

                // 2. 그나마 큰놈을 삭제할려던 놈 부모 노드와 연결
                prev -> right = deep;

                // 3. 그나마 큰 놈의 오른쪽 자식노드로 temp의 오른쪽 노드를 연결
                deep -> right = temp -> right;

                // 4. 중요!!!!!!!!: 그나마 큰 놈의 왼쪽 자식노드로 temp의 왼쪽 노드를 연결
                deep -> left = temp -> left;

                // 5. 삭제할려던 놈 삭제
                free(temp);
            }
            break;
    }
}

// 이진 트리 출력 (중위 순회)
void inorderTraversal(TreeNode* root){
    if(root == NULL){
        return;
    }
    inorderTraversal(root->left);
    printf("%d -> ", root->key);
    inorderTraversal(root->right);
}

// 이진 트리 메모리 해제
void freeTree(TreeNode* root) {
    if (root == NULL) {
        return; // 빈 트리이면 아무 것도 하지 않고 반환
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root); // left,right 해제하고 root를 해제해야 함
}

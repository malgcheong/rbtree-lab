#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

/* 
RULE ---------------------------------------------
1. 노드는 레드 혹은 블랙 중의 하나이다.
2. 루트 노드는 블랙이다.
3. 모든 리프 노드들(NIL)은 블랙이다.
4. 레드 노드의 자식노드 양쪽은 언제나 모두 블랙이다.
5. 어떤 노드로부터 시작되어 그에 속한 하위 리프 노드에 도달하는 모든 경로에는 리프 노드를 제외하면 모두 같은 개수의 블랙 노드가 있다.
-----------------------------------------------------

INSERT ---------------------------------------------
|Case|
Case 1. Z = root
Case 2. Z.uncle = red
Case 3. Z.uncle = black(triangle)
Case 4. Z.uncle = black(line)

|Point|
1. 5번 규칙을 우선 지키기 위해(5번 규칙이 제일 까다롭기 때문) 새로운 노드는 무조건 빨간색 노드로 넣게 된다.
2. 그 결과 4번 규칙 위반이 대부분 발생한다. (새로 넣은 z노드가 루트 노드인 경우를 제외하고 모두)
3. 4번 규칙 위반을 해결하기 위해서 부모의 빨간 노드를 건너편(왼쪽 or 오른쪽)으로 넘겨주는 rotate를 실행한다.
3-1. 만약 반대편에도 빨간 노드(삼촌)가 이미 존재한다면 rotate는 하지 않고 부모,삼촌과 할아버지 색깔만 바꿔준다. 그럼으로써 할아버지부터 z까지는 4번 규칙을 우선 지키게 한다.
3-2. rotate를 실행해주는 경우에는 빨간 노드를 건너편으로 넘겨주기 때문에 할아버지(Black) 위의 트리에서 4번 규칙 위반이 발생하지 않는다. 즉 rotate가 발생했다면 할아버지는 똑같이 Black이므로 할아버지 위의 트리는 검증하지 않아도 된다.
3-3. rotate를 하지 않고 부모,삼촌은 Black으로 할아버지는 Red로 바꿔준다면 할아버지(Red)부터 다시 위쪽으로 검증이 필요해진다. (할아버지와 증조할아버지 모두 Red로 4번 규칙 위반 가능성이 있으므로)
4. Case 4 line의 경우엔 rotate가 한 번 일어난다.
4-1. 할아버지 기준으로 rotate가 이루어지면 부모 노드가 새로운 할아버지가 되고 새로운 할아버지는 똑같이 Black이므로 할아버지 위의 트리는 검증하지 않아도 된다.
5. Case 3 triangle의 경우엔 rotate가 두 번 일어난다.
5-1. rotate가 두 번 일어나는 이유는 한 번의 rotate로 Case 4 line을 만들어주기 위해서이다.
5-2. 부모 기준으로 rotate가 한 번 일어나면 부모 노드는 새로운 자식 노드가 된다. 그 새로운 자식 노드의 할아버지 기준으로 두 번째 rotate를 돌려줘야 한다. 그것이 4-1번과 동일하다.
5-3. 즉 첫 번째 부모 기준으로 rotate가 이루어지고 부모 노드가 z가 되어서 while문을 돌면 z의 할아버지 기준으로 Case 4 line rotate가 진행된다.

|Fact|
1. 규칙들을 만족하면서 insert가 진행된다면 할아버지 노드는 무조건 Black이다.
2. 규칙들을 만족하면서 insert가 진행된다면 할아버지 노드(black)와 부모 노드(Red), 삼촌 노드(Red)를 바꿔줘도 5번 규칙은 변함없이 유지된다. (할아버지 위의 트리와 삼촌 밑의 서브트리는 이전에 insert 진행하면서 검증했을 것이며, 부모 밑의 서브트리는 이번 insert 진행하면서 검증했을 것이다.)
-----------------------------------------------------

*/
#ifdef SENTINEL
rbtree *new_rbtree(void) {
  // TODO: initialize struct if needed
  // calloc하면 구조체 안의 변수 값도 NULL로 세팅
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t * nilNode = (node_t *)calloc(1, sizeof(node_t));
  nilNode -> color = RBTREE_BLACK;
  p -> root = nilNode;
  p -> nil = nilNode;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *root = t -> root; 
  delete_recursive_node_t(t, root);
  free(t -> nil);
  free(t);
}

void delete_recursive_node_t(rbtree *t, node_t * node) {
  // TODO: reclaim the tree nodes's memory
  if( node == NULL){
    return;
  }
  delete_recursive_node_t(t, node -> left);
  delete_recursive_node_t(t, node -> right);
  if (node != t -> nil){
    free(node);
  }
}

node_t * rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *temp = t -> root;
  node_t *prev = NULL;

  while (temp != t -> nil){
      prev = temp;
      if (key >= temp -> key){
          temp = temp -> right;
      } else if (key < temp -> key){
          temp = temp -> left;
      }
  }
  node_t * newNode = (node_t *)calloc(1, sizeof(node_t));
  newNode -> key = key;

  // 트리가 비어있는 경우 바로 루트노르로 반환
  if (prev == NULL) {
      t -> root = newNode;
      newNode -> parent = t -> nil;
  }
  //찾은 곳 이전 노드의 오른쪽에 새 노드 연결
  else if (key >= prev->key) {
      prev->right = newNode;
      newNode -> parent = prev;
  } 
  //찾은 곳 이전 노드의 왼쪽에 새 노드 연결
  else {
      prev->left = newNode;
      newNode -> parent = prev;
  }
  newNode -> color = RBTREE_RED; // RB 트리에서 삽입되는 새로운 노드의 색은 RED이다
  newNode -> left = t -> nil;
  newNode -> right = t -> nil;

  rbtree_insert_fixup(t, newNode);
  return t->root;
}

void rbtree_insert_fixup(rbtree * t, node_t * z){
  // TODO: Fixup color and rotate
  if (t -> root == z){
    // CASE 1: z = root
    t -> root -> color = RBTREE_BLACK; // 2. 루트 노드는 블랙이다.
    return;
  }
  while (z -> parent != NULL && z -> parent -> color == RBTREE_RED){
  	// 조부모가 없는 상황은 부모가 루트 노드라는 뜻이며 루트 노드는 블랙이므로 while문을 탈 수 없다.
    if (z -> parent == z -> parent -> parent -> right) {
        node_t *grandPa = z -> parent -> parent;
        node_t *parent = grandPa -> right;

        // CASE 2: z.uncle = red
        if(grandPa -> left != NULL && grandPa -> left -> color == RBTREE_RED){
          node_t *uncle = grandPa -> left;
          if (uncle -> color == RBTREE_RED){
            grandPa -> color = RBTREE_RED;
            uncle -> color = RBTREE_BLACK;
            parent -> color = RBTREE_BLACK;
            z = grandPa;
          }
        }
        else if (grandPa -> left == NULL || grandPa -> left -> color == RBTREE_BLACK){
          // CASE 3: z.uncle = black (triangle) 
          if ( parent -> left == z){
            rbtree_right_rotate(t, parent);
            z = parent;
          }
          // CASE 4: z.uncle = black (line)
          else if ( parent -> right == z){
            grandPa -> color = RBTREE_RED;
            parent -> color = RBTREE_BLACK;
            rbtree_left_rotate(t, grandPa);
            break;
          }
        } 
    } 
    else if (z -> parent == z -> parent -> parent -> left) {
      node_t *grandPa = z -> parent -> parent;
      node_t *parent = grandPa -> left;

      // CASE 2: z.uncle = red
      if(grandPa -> right != NULL && grandPa -> right -> color == RBTREE_RED){
        node_t *uncle = grandPa -> right;
        if (uncle -> color == RBTREE_RED){
          grandPa -> color = RBTREE_RED;
          uncle -> color = RBTREE_BLACK;
          parent -> color = RBTREE_BLACK;
          z = grandPa;
        }
      }
      else if (grandPa -> right == NULL || grandPa -> right -> color == RBTREE_BLACK){
        // CASE 3: z.uncle = black (triangle) 
        if ( parent -> right == z){
          rbtree_left_rotate(t, parent);
          z = parent;
        }
        // CASE 4: z.uncle = black (line)
        else if ( parent -> left == z){
          grandPa -> color = RBTREE_RED;
          parent -> color = RBTREE_BLACK;
          rbtree_right_rotate(t, grandPa);
          break;
        }
      } 
    }
  }
  // CASE 1: z = root
  t -> root -> color = RBTREE_BLACK; // 2. 루트 노드는 블랙이다.
}

void rbtree_left_rotate(rbtree * t, node_t * z){
  node_t *newGrandPa = z -> right;
  z -> right = newGrandPa -> left;
  newGrandPa -> parent = z -> parent;
  
  if (newGrandPa -> left != NULL) {
   newGrandPa -> left -> parent = z;
  }
  
  newGrandPa -> left = z;
  
  if (z -> parent == NULL) {
    t -> root = newGrandPa;
  } else if (z == z -> parent -> left) {
    z -> parent -> left = newGrandPa;
  } else if (z == z -> parent -> right) {
    z -> parent -> right = newGrandPa;
  }
  
  z -> parent = newGrandPa;
}

void rbtree_right_rotate(rbtree * t, node_t * z){
  node_t *newGrandPa = z -> left;
  z -> left = newGrandPa -> right;
  newGrandPa -> parent = z -> parent;
  
  if (newGrandPa -> right != NULL) {
   newGrandPa -> right -> parent = z;
  }
  
  newGrandPa -> right = z;
  
  if (z -> parent == NULL) {
    t -> root = newGrandPa;
  } else if (z == z -> parent -> right) {
    z -> parent -> right = newGrandPa;
  } else if (z == z -> parent -> left) {
    z -> parent -> left = newGrandPa;
  }
  
  z -> parent = newGrandPa;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *temp = t -> root;
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

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

#else

rbtree *new_rbtree(void) {
  // TODO: initialize struct if needed
  // calloc하면 구조체 안의 변수 값도 NULL로 세팅
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *root = t -> root; 
  delete_recursive_node_t(t, root);
  free(t);
}

void delete_recursive_node_t(rbtree *t, node_t * node){
  // TODO: reclaim the tree nodes's memory
  if( node == NULL){
    return;
  }
  delete_recursive_node_t(t, node -> left);
  delete_recursive_node_t(t, node -> right);
  free(node);
}

node_t * rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *temp = t -> root;
  node_t *prev = NULL;

  while (temp){
      prev = temp;
      if (key >= temp -> key){
          temp = temp -> right;
      } else if (key < temp -> key){
          temp = temp -> left;
      }
  }
  node_t * newNode = (node_t *)calloc(1, sizeof(node_t));
  newNode -> key = key;

  // 트리가 비어있는 경우 바로 루트노르로 반환
  if (prev == NULL) {
      t -> root = newNode;
  }
  //찾은 곳 이전 노드의 오른쪽에 새 노드 연결
  else if (key >= prev->key) {
      prev->right = newNode;
      newNode -> parent = prev;
  } 
  //찾은 곳 이전 노드의 왼쪽에 새 노드 연결
  else {
      prev->left = newNode;
      newNode -> parent = prev;
  }
  newNode -> color = RBTREE_RED; // RB 트리에서 삽입되는 새로운 노드의 색은 RED이다
  newNode -> left = NULL;
  newNode -> right = NULL;

  rbtree_insert_fixup(t, newNode);
  return t->root;
}

void rbtree_insert_fixup(rbtree * t, node_t * z){
  // TODO: Fixup color and rotate
  if (t -> root == z){
    // CASE 1: z = root
    t -> root -> color = RBTREE_BLACK; // 2. 루트 노드는 블랙이다.
    return;
  }
  while (z -> parent != NULL && z -> parent -> color == RBTREE_RED){
  	// 조부모가 없는 상황은 부모가 루트 노드라는 뜻이며 루트 노드는 블랙이므로 while문을 탈 수 없다.
    if (z -> parent == z -> parent -> parent -> right) {
        node_t *grandPa = z -> parent -> parent;
        node_t *parent = grandPa -> right;

        // CASE 2: z.uncle = red
        if(grandPa -> left != NULL && grandPa -> left -> color == RBTREE_RED){
          node_t *uncle = grandPa -> left;
          if (uncle -> color == RBTREE_RED){
            grandPa -> color = RBTREE_RED;
            uncle -> color = RBTREE_BLACK;
            parent -> color = RBTREE_BLACK;
            z = grandPa;
          }
        }
        else if (grandPa -> left == NULL || grandPa -> left -> color == RBTREE_BLACK){
          // CASE 3: z.uncle = black (triangle) 
          if ( parent -> left == z){
            rbtree_right_rotate(t, parent);
            z = parent;
          }
          // CASE 4: z.uncle = black (line)
          else if ( parent -> right == z){
            grandPa -> color = RBTREE_RED;
            parent -> color = RBTREE_BLACK;
            rbtree_left_rotate(t, grandPa);
            break;
          }
        } 
    } 
    else if (z -> parent == z -> parent -> parent -> left) {
      node_t *grandPa = z -> parent -> parent;
      node_t *parent = grandPa -> left;

      // CASE 2: z.uncle = red
      if(grandPa -> right != NULL && grandPa -> right -> color == RBTREE_RED){
        node_t *uncle = grandPa -> right;
        if (uncle -> color == RBTREE_RED){
          grandPa -> color = RBTREE_RED;
          uncle -> color = RBTREE_BLACK;
          parent -> color = RBTREE_BLACK;
          z = grandPa;
        }
      }
      else if (grandPa -> right == NULL || grandPa -> right -> color == RBTREE_BLACK){
        // CASE 3: z.uncle = black (triangle) 
        if ( parent -> right == z){
          rbtree_left_rotate(t, parent);
          z = parent;
        }
        // CASE 4: z.uncle = black (line)
        else if ( parent -> left == z){
          grandPa -> color = RBTREE_RED;
          parent -> color = RBTREE_BLACK;
          rbtree_right_rotate(t, grandPa);
          break;
        }
      } 
    }
  }
  // CASE 1: z = root
  t -> root -> color = RBTREE_BLACK; // 2. 루트 노드는 블랙이다.
}

void rbtree_left_rotate(rbtree * t, node_t * z){
  node_t *newGrandPa = z -> right;
  z -> right = newGrandPa -> left;
  newGrandPa -> parent = z -> parent;
  
  if (newGrandPa -> left != NULL) {
   newGrandPa -> left -> parent = z;
  }
  
  newGrandPa -> left = z;
  
  if (z -> parent == NULL) {
    t -> root = newGrandPa;
  } else if (z == z -> parent -> left) {
    z -> parent -> left = newGrandPa;
  } else if (z == z -> parent -> right) {
    z -> parent -> right = newGrandPa;
  }
  
  z -> parent = newGrandPa;
}

void rbtree_right_rotate(rbtree * t, node_t * z){
  node_t *newGrandPa = z -> left;
  z -> left = newGrandPa -> right;
  newGrandPa -> parent = z -> parent;
  
  if (newGrandPa -> right != NULL) {
   newGrandPa -> right -> parent = z;
  }
  
  newGrandPa -> right = z;
  
  if (z -> parent == NULL) {
    t -> root = newGrandPa;
  } else if (z == z -> parent -> right) {
    z -> parent -> right = newGrandPa;
  } else if (z == z -> parent -> left) {
    z -> parent -> left = newGrandPa;
  }
  
  z -> parent = newGrandPa;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *temp = t -> root;
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

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

#endif
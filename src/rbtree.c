#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
// #define SENTINEL

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

DELETE ---------------------------------------------
|Replaced Node|
1. 삭제할려는 노드가 리프노드일 경우
-> 대체하지 않아도 된다.

2. 자식 노드가 하나인 경우
-> 자식 노드로 대체된다. 
(successor로 대체하지 않고 바로 자식노드로 대체 되는 이유는 
RB tree insert는 4번 속성 때문에 자체적으로 밸런싱을 잡기 때문에
자식 노드가 두개 모두 있지 않는 이상 더 아래에 succeessor가 존재할 수 없다)

3. 자식 노드가 모두 있는 경우
-> 삭제되는 노드의 successor로 대체된다.

|Doubly Black solve case|
case 4. doubly black의 오른쪽 형제가 black & 그 형제의 오른쪽 자녀가 red일 때
->  그 red를 doubly black 위로 옮기고 
    그 red로 extra black을 전달해서 
    red-and-black으로 만들면 
    red-and-black을 black으로 바꿔서 해결

case 3. doubly black의 오른쪽 형제가 black & 그 형제의 왼쪽 자녀가 red & 그 형제의 오른쪽 자녀가 black일 때
->  doubly black의 형제의 오른쪽 자녀가 
    red가 되게 만들어서 
    이후엔 case 4를 적용하여 해결

case 2. doubly black의 형제가 black & 그 형제의 두 자녀 모두 black일 때
->  doubly black과 그 형제의 black을 모아서 부모에게 전달해서 
    부모가 extra black을 해결하도록 위임한다.
    그 부모가 red-and-black이 됐다면 black으로 바꿔주면 상황은 종료되지만 
    그 부모가 doubly black이 됐다면 B가 루트 노드라면 black으로 바꿔서 해결, 
    아니라면 case 1,2,3,4 중에 하나로 해결

case 1. doubly black의 형제가 red일 때
->  doubly black의 형제를 black으로 만든 후 
    case 2,3,4 중에 하나로 해결
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
  if(node != t -> nil){
    delete_recursive_node_t(t, node -> left);
    delete_recursive_node_t(t, node -> right);
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
            if (parent -> parent == t -> nil){
              t -> root = parent;
            }
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
          if (parent -> parent == t -> nil){
            t -> root = parent;
          }
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
  
  if (newGrandPa -> left != t -> nil) {
   newGrandPa -> left -> parent = z;
  }
  
  newGrandPa -> left = z;
  
  if (z -> parent == t -> nil) {
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
  
  if (newGrandPa -> right != t -> nil) {
   newGrandPa -> right -> parent = z;
  }
  
  newGrandPa -> right = z;
  
  if (z -> parent == t -> nil) {
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
  node_t *root = t -> root;

  node_t *temp = root;
  node_t *successor = NULL;
  while(temp != t -> nil){
    successor = temp;
    if( temp -> left != t -> nil){
      temp = temp -> left;
    } else{
      break;
    }
  }
  return successor;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *root = t -> root;

  node_t *temp = root;
  node_t *successor = NULL;
  while(temp != t -> nil){
    successor = temp;
    if( temp -> right != t -> nil){
      temp = temp -> right;
    } else{
      break;
    }
  }
  return successor;
}

// doubly black과 red black을 fixup해주는 함수이다.
void rbtree_delete_fixup(rbtree *t, node_t *z){

  /** 이 z는 doubly black 노드이거나 red black 노드이다.
  *  왜냐하면 black 색깔 노드가 삭제된 경우에 이 함수를 타기 때문이다.
  *  while문을 타는 놈은 doubly black인 놈을 제거하는 것이고
  *  while문 조건을 종료하거나 아예 안 타는 놈은 red black 노드에서 black을 제거해야 하는 놈이다.
  */
  while (z != t -> root && z -> color == RBTREE_BLACK)  {

    if (z == z -> parent -> left){
      node_t *sibling = z -> parent -> right;
      // case 1
      if (sibling -> color == RBTREE_RED){
        sibling -> color = RBTREE_BLACK;
        sibling -> parent -> color = RBTREE_RED;
        rbtree_left_rotate(t, sibling -> parent);
        sibling = z -> parent -> right;
      }
      // case 2
      if (sibling -> left -> color == RBTREE_BLACK && sibling -> right -> color == RBTREE_BLACK){
        sibling -> color = RBTREE_RED;
        z = z -> parent;
      }
      else{
        // case 3
        if (sibling -> right -> color == RBTREE_BLACK){
          sibling -> left -> color = RBTREE_BLACK;
          sibling -> color = RBTREE_RED;
          rbtree_right_rotate(t, sibling);
          sibling = z -> parent -> right;
        }
        // case 4
        sibling -> color = z -> parent -> color;
        z -> parent -> color = RBTREE_BLACK;
        sibling -> right -> color = RBTREE_BLACK;
        rbtree_left_rotate(t, z->parent);
        z = t -> root;
      }
    }

    else {
      node_t *sibling = z -> parent -> left;
      // case 1
      if (sibling -> color == RBTREE_RED){
        sibling -> color = RBTREE_BLACK;
        sibling -> parent -> color = RBTREE_RED;
        rbtree_right_rotate(t, sibling -> parent);
        sibling = z -> parent -> left;
      }
      // case 2
      if (sibling -> right -> color == RBTREE_BLACK && sibling -> left -> color == RBTREE_BLACK){
        sibling -> color = RBTREE_RED;
        z = z -> parent;
      }
      else{
        // case 3
        if (sibling -> left -> color == RBTREE_BLACK){
          sibling -> right -> color = RBTREE_BLACK;
          sibling -> color = RBTREE_RED;
          rbtree_left_rotate(t, sibling);
          sibling = z -> parent -> left;
        }
        // case 4
        sibling -> color = z -> parent -> color;
        z -> parent -> color = RBTREE_BLACK;
        sibling -> left -> color = RBTREE_BLACK;
        rbtree_right_rotate(t, z->parent);
        z = t -> root;
      }
    }
  }
  // red black 노드는 그냥 black 노드로 바꿔줘도 무방하다.
  z -> color = RBTREE_BLACK;
}

void rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *replaced_node;
  color_t delete_or_successor_color;

  // 삭제되는 노드의 오른쪽 자식이 있는 경우 오른쪽 자식이 대체하고
  // 색깔은 삭제되는 노드 z의 색깔을 그대로 쓴다.
  if (z -> left == t -> nil) {
    replaced_node = z -> right;
    delete_or_successor_color = z -> color;
    rbtree_transplant(t, z, z -> right);
  }
  // 삭제되는 노드의 왼쪽 자식이 있는 경우 왼쪽 자식이 대체하고
  // 색깔은 삭제되는 노드 z의 색깔을 그대로 쓴다.
  else if (z -> right == t -> nil) {
    replaced_node = z -> left; 
    delete_or_successor_color = z -> color;
    rbtree_transplant(t, z, z -> left);
  }

  else {
    node_t *successor = bst_min(t, z -> right);
    replaced_node = successor -> right;
    delete_or_successor_color = successor -> color;
    // successor가 더 깊이 들어갈 경우 교체될 때, 오른쪽도 갈아끼워줘야 한다.
    // successor가 z 바로 오른쪽이면 왼쪽만 갈아끼우면 되는데, 더 깊이 들어가면 오른쪽도 갈아끼워줘야 한다. 이건 bst 삭제에서 핵심이었다.
    if (successor != z -> right) {
      rbtree_transplant(t, successor, successor -> right);
      successor -> right = z -> right;
      successor -> right -> parent = successor;
    }
    else{
      // 이 함수에서 해주는 parent연결만 아래처럼 해주는 것, rbtree_transplant(t, successor, successor -> right);
      // 위의 if문은 successor가 깊이 있어서 successor가 삭제되면  successor의 부모한테 successor-> right가 붙어야 하고
      // 지금 else문은 successor가 삭제할 놈 바로 오른쪽 놈이기 때문에 successor의 오른쪽 nil 노드는 바로 삭제되는 노드에 대체되는 successor의 오른쪽에 바로 붙으면 되므로 parent를 successor로 한다.
      replaced_node -> parent = successor;
    }
    rbtree_transplant(t, z, successor);
    successor -> left = z -> left;
    successor -> left -> parent = successor;
    successor -> color = z -> color;
  }

  /* 삭제된 노드의 색깔이 블랙일 때, fixup이 진행되는 이유
    - 삭제되는 노드 또는 successor의 색이 black이면 
      그 노드 위치가 무슨 색이냐에 따라 red black node, doubled black node
      가 적용되어서 5번 속성을 준수하겠지만
      red black node, doubled black node을 없애주기 위한 fixup 과정이 필요하다
    
    - 삭제되는 노드 또는 successor의 색이 red이라면 
      그 노드는 그냥 삭제해도 무방하다. 
      red 노드는 지워도 4,5번 속성을 위반하지 않기 때문
  */
  if (delete_or_successor_color == RBTREE_BLACK){
    rbtree_delete_fixup(t, replaced_node);
  }
  t -> nil -> parent = NULL;
  free(z);
}


/**
 * 이 함수는 이진 탐색 트리에서 노드를 대체하는 데 사용됩니다.
 * u는 삭제될 노드
 * v는 삭제된 노드를 대체할 노드
 */
void rbtree_transplant(rbtree *t, node_t *u, node_t *v){ 
  // 삭제할 노드가 루트노드일 경우
  if (u -> parent == t -> nil){
    t -> root = v;
  }
  // 삭제할 노드가 부모노드의 왼쪽 자식일 경우
  else if (u == u -> parent -> left){
    u -> parent -> left = v;
  }
  // 삭제할 노드가 부모노드의 왼쪽 자식일 경우
  else if (u == u -> parent -> right){  
    u -> parent -> right = v;
  }

  v -> parent = u -> parent;
}

node_t *bst_min(const rbtree *t, node_t *z) {
  node_t *temp = z;
  node_t *successor = NULL;
  while(temp != t -> nil){
    successor = temp;
    if( temp -> left != t -> nil){
      temp = temp -> left;
    } else{
      break;
    }
  }
  return successor;
}

// 이진 트리 출력 (중위 순회)
void bst_inorderTraversal(const rbtree *t){
  bst_inorderTraversal_recursive(t, t->root);
  printf("\n");
}

void bst_inorderTraversal_recursive(const rbtree *t, node_t *root){
  if(root == t -> nil){
      return;
  }
  bst_inorderTraversal_recursive(t, root->left);
  printf("%d -> ", root->key);
  bst_inorderTraversal_recursive(t, root->right);
}

void bst_inorderTraversal_recursive_to_array(const rbtree *t, node_t *root, key_t *arr, key_t *limit, const size_t n){
  if(root != t -> nil && *limit < n){
    bst_inorderTraversal_recursive_to_array(t, root->left, arr, limit, n);
    arr[(*limit)++] = root -> key;
    bst_inorderTraversal_recursive_to_array(t, root->right, arr, limit, n);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  key_t lm = 0; 
  key_t *limit = &lm;
  bst_inorderTraversal_recursive_to_array(t, t-> root, arr, limit, n);
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
            if (parent -> parent == NULL){
              t -> root = parent;
            }
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
          if (parent -> parent == NULL){
            t -> root = parent;
          }
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
  node_t *root = t -> root;

  node_t *temp = root;
  node_t *successor = NULL;
  while(temp != NULL){
    successor = temp;
    if( temp -> left != NULL){
      temp = temp -> left;
    } else{
      break;
    }
  }
  return successor;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *root = t -> root;

  node_t *temp = root;
  node_t *successor = NULL;
  while(temp != NULL){
    successor = temp;
    if( temp -> right != NULL){
      temp = temp -> right;
    } else{
      break;
    }
  }
  return successor;
}

// doubly black과 red black을 fixup해주는 함수이다.
void rbtree_delete_fixup(rbtree *t, node_t *z){
  /** 이 z는 doubly black 노드이거나 red black 노드이다.
  *  왜냐하면 black 색깔 노드가 삭제된 경우에 이 함수를 타기 때문이다.
  *  while문을 타는 놈은 doubly black인 놈을 제거하는 것이고
  *  while문 조건을 종료하거나 아예 안 타는 놈은 red black 노드에서 black을 제거해야 하는 놈이다.
  */
  while (z != t -> root && z -> color == RBTREE_BLACK)  {

    if (z == z -> parent -> left){
      node_t *sibling = z -> parent -> right;
      // case 1
      if (sibling -> color == RBTREE_RED){
        sibling -> color = RBTREE_BLACK;
        sibling -> parent -> color = RBTREE_RED;
        rbtree_left_rotate(t, sibling -> parent);
        sibling = z -> parent -> right;
      }
      // case 2
      if (sibling -> left -> color == RBTREE_BLACK && sibling -> right -> color == RBTREE_BLACK){
        sibling -> color = RBTREE_RED;
        z = z -> parent;
      }
      else{
        // case 3
        if (sibling -> right -> color == RBTREE_BLACK){
          sibling -> left -> color = RBTREE_BLACK;
          sibling -> color = RBTREE_RED;
          rbtree_right_rotate(t, sibling);
          sibling = z -> parent -> right;
        }
        // case 4
        sibling -> color = z -> parent -> color;
        z -> parent -> color = RBTREE_BLACK;
        sibling -> right -> color = RBTREE_BLACK;
        rbtree_left_rotate(t, z->parent);
        z = t -> root;
      }
    }

    else {
      node_t *sibling = z -> parent -> left;
      // case 1
      if (sibling -> color == RBTREE_RED){
        sibling -> color = RBTREE_BLACK;
        sibling -> parent -> color = RBTREE_RED;
        rbtree_right_rotate(t, sibling -> parent);
        sibling = z -> parent -> left;
      }
      // case 2
      if (sibling -> right -> color == RBTREE_BLACK && sibling -> left -> color == RBTREE_BLACK){
        sibling -> color = RBTREE_RED;
        z = z -> parent;
      }
      else{
        // case 3
        if (sibling -> left -> color == RBTREE_BLACK){
          sibling -> right -> color = RBTREE_BLACK;
          sibling -> color = RBTREE_RED;
          rbtree_left_rotate(t, sibling);
          sibling = z -> parent -> left;
        }
        // case 4
        sibling -> color = z -> parent -> color;
        z -> parent -> color = RBTREE_BLACK;
        sibling -> left -> color = RBTREE_BLACK;
        rbtree_right_rotate(t, z->parent);
        z = t -> root;
      }
    }
  }

  // red black 노드는 그냥 black 노드로 바꿔줘도 무방하다.
  if(z != NULL){
    z -> color = RBTREE_BLACK;
  }
}

void rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *replaced_node;
  color_t delete_or_successor_color;

  // 삭제되는 노드의 오른쪽 자식이 있는 경우 오른쪽 자식이 대체하고
  // 색깔은 삭제되는 노드 z의 색깔을 그대로 쓴다.
  if (z -> left == NULL) {
    replaced_node = z -> right;
    delete_or_successor_color = z -> color;
    rbtree_transplant(t, z, z -> right);
  }
  // 삭제되는 노드의 왼쪽 자식이 있는 경우 왼쪽 자식이 대체하고
  // 색깔은 삭제되는 노드 z의 색깔을 그대로 쓴다.
  else if (z -> right == NULL) {
    replaced_node = z -> left; 
    delete_or_successor_color = z -> color;
    rbtree_transplant(t, z, z -> left);
  }

  else {
    node_t *successor = bst_min(t, z -> right);
    replaced_node = successor -> right;
    delete_or_successor_color = successor -> color;
    // successor가 더 깊이 들어갈 경우 교체될 때, 오른쪽도 갈아끼워줘야 한다.
    // successor가 z 바로 오른쪽이면 왼쪽만 갈아끼우면 되는데, 더 깊이 들어가면 오른쪽도 갈아끼워줘야 한다. 이건 bst 삭제에서 핵심이었다.
    if (successor != z -> right) {
      rbtree_transplant(t, successor, successor -> right);
      successor -> right = z -> right;
      successor -> right -> parent = successor;
    }
    else{
      // 이 함수에서 해주는 parent연결만 아래처럼 해주는 것, rbtree_transplant(t, successor, successor -> right);
      // 위의 if문은 successor가 깊이 있어서 successor가 삭제되면  successor의 부모한테 successor-> right가 붙어야 하고
      // 지금 else문은 successor가 삭제할 놈 바로 오른쪽 놈이기 때문에 successor의 오른쪽 nil 노드는 바로 삭제되는 노드에 대체되는 successor의 오른쪽에 바로 붙으면 되므로 parent를 successor로 한다.
      replaced_node -> parent = successor;
    }
    rbtree_transplant(t, z, successor);
    successor -> left = z -> left;
    successor -> left -> parent = successor;
    successor -> color = z -> color;
  }

  /* 삭제된 노드의 색깔이 블랙일 때, fixup이 진행되는 이유
    - 삭제되는 노드 또는 successor의 색이 black이면 
      그 노드 위치가 무슨 색이냐에 따라 red black node, doubled black node
      가 적용되어서 5번 속성을 준수하겠지만
      red black node, doubled black node을 없애주기 위한 fixup 과정이 필요하다
    
    - 삭제되는 노드 또는 successor의 색이 red이라면 
      그 노드는 그냥 삭제해도 무방하다. 
      red 노드는 지워도 4,5번 속성을 위반하지 않기 때문
  */
  if (delete_or_successor_color == RBTREE_BLACK){
    rbtree_delete_fixup(t, replaced_node);
  }
  // t -> nil -> parent = NULL;
  free(z);
}


/**
 * 이 함수는 이진 탐색 트리에서 노드를 대체하는 데 사용됩니다.
 * u는 삭제될 노드
 * v는 삭제된 노드를 대체할 노드
 */
void rbtree_transplant(rbtree *t, node_t *u, node_t *v){ 
  // 삭제할 노드가 루트노드일 경우
  if (u -> parent == NULL){
    t -> root = v;
  }
  // 삭제할 노드가 부모노드의 왼쪽 자식일 경우
  else if (u == u -> parent -> left){
    u -> parent -> left = v;
  }
  // 삭제할 노드가 부모노드의 왼쪽 자식일 경우
  else if (u == u -> parent -> right){  
    u -> parent -> right = v;
  }

  if (v != NULL){
    v -> parent = u -> parent;
  }
}

node_t *bst_min(const rbtree *t, node_t *z) {
  node_t *temp = z;
  node_t *successor = NULL;
  while(temp != NULL){
    successor = temp;
    if( temp -> left != NULL){
      temp = temp -> left;
    } else{
      break;
    }
  }
  return successor;
}

// 이진 트리 출력 (중위 순회)
void bst_inorderTraversal(const rbtree *t){
  bst_inorderTraversal_recursive(t, t->root);
  printf("\n");
}

void bst_inorderTraversal_recursive(const rbtree *t, node_t *root){
  if(root == NULL){
      return;
  }
  bst_inorderTraversal_recursive(t, root->left);
  printf("%d -> ", root->key);
  bst_inorderTraversal_recursive(t, root->right);
}

void bst_inorderTraversal_recursive_to_array(const rbtree *t, node_t *root, key_t *arr, key_t *limit, const size_t n){
  if(root != NULL && *limit < n){
    bst_inorderTraversal_recursive_to_array(t, root->left, arr, limit, n);
    arr[(*limit)++] = root -> key;
    bst_inorderTraversal_recursive_to_array(t, root->right, arr, limit, n);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  key_t lm = 0; 
  key_t *limit = &lm;
  bst_inorderTraversal_recursive_to_array(t, t-> root, arr, limit, n);
  return 0;
}


#endif
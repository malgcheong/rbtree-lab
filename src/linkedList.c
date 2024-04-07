#include <stdio.h>
#include <stdlib.h>

// 노드 구조체 정의
struct Node {
    int data; 
    struct Node* next;
};

// 새로운 노드 생성 함수
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// 리스트의 머리에 노드 삽입 함수
void insertAtHead(struct Node** headRef, int data) {
    struct Node* newNode = createNode(data);
    newNode->next = *headRef;
    *headRef = newNode;
}

// 리스트의 꼬리에 노드 삽입 함수
void insertAtTail(struct Node** headRef, int data) {
    struct Node* newNode = createNode(data);
    printf("%p", &newNode);
    if (*headRef == NULL) {
        *headRef = newNode;
        return;
    }
    struct Node* current = *headRef;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

// 리스트에서 특정 위치에 노드 삽입 함수
void insertAtPosition(struct Node** headRef, int data, int position) {
    if (position < 0) {
        printf("Invalid position\n");
        return;
    }
    if (position == 0) {
        insertAtHead(headRef, data);
        return;
    }
    struct Node* newNode = createNode(data);
    struct Node* current = *headRef;
    int count = 0;
    while (current != NULL && count < position - 1) {
        current = current->next;
        count++;
    }
    if (current == NULL) {
        printf("Position out of range\n");
        return;
    }
    newNode->next = current->next;
    current->next = newNode;
}

// 리스트에서 특정 위치의 노드 삭제 함수
void deleteAtPosition(struct Node** headRef, int position) {
    if (*headRef == NULL) {
        printf("List is empty\n");
        return;
    }
    struct Node* temp = *headRef;
    if (position == 0) {
        *headRef = temp->next;
        free(temp);
        return;
    }
    int count = 0;
    struct Node* prev = NULL;
    while (temp != NULL && count < position) {
        prev = temp;
        temp = temp->next;
        count++;
    }
    if (temp == NULL) {
        printf("Position out of range\n");
        return;
    }
    prev->next = temp->next;
    free(temp);
}

// 리스트에서 특정 값을 가진 노드 검색 함수
struct Node* search(struct Node* head, int key) {
    struct Node* current = head;
    while (current != NULL) {
        if (current->data == key) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 리스트 출력 함수
void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

int main() {
    struct Node* head = NULL;

    // 노드 삽입
    insertAtTail(&head, 1);
    insertAtTail(&head, 2);
    insertAtTail(&head, 3);
    insertAtHead(&head, 0);
    insertAtPosition(&head, 5, 3);

    // 리스트 출력
    printf("Linked List: ");
    printList(head);

    // 노드 삭제
    deleteAtPosition(&head, 2);

    // 리스트 출력
    printf("Linked List after deletion: ");
    printList(head);

    // 특정 값 검색
    int key = 3;
    struct Node* result = search(head, key);
    if (result != NULL) {
        printf("%d found in the list\n", key);
    } else {
        printf("%d not found in the list\n", key);
    }

    return 0;
}

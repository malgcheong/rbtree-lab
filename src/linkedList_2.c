#include <stdio.h>
#include <stdlib.h>

// 단일 연결 리스트의 노드 구조체 정의
typedef struct Node {
    int data;
    struct Node *next;
} Node;

// 리스트와 관련된 함수 선언
void insertAtHead(Node *head, int data);
void insertAtTail(Node *head, int data);
void printList(Node *head);

int main() {
    // 더미 노드 생성
    Node *dummy = (Node *)malloc(sizeof(Node));
    if (dummy == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    dummy->next = NULL; // 더미 노드의 다음 노드를 NULL로 초기화

    // 더미 노드를 헤드로 사용
    Node *head = dummy;

    // 노드 추가
    insertAtHead(head, 10);
    insertAtTail(head, 20);
    insertAtTail(head, 30);

    // 리스트 출력
    printList(head->next);

    // 더미 노드 해제
    free(dummy);

    return 0;
}

// 리스트의 머리에 노드 삽입
void insertAtHead(Node *head, int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    newNode->data = data;
    newNode->next = head->next;
    head->next = newNode;
}

// 리스트의 꼬리에 노드 삽입
void insertAtTail(Node *head, int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    newNode->data = data;
    newNode->next = NULL;

    // 마지막 노드 찾기
    Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    // 마지막 노드의 다음에 새로운 노드 연결
    current->next = newNode;
}

// 리스트 출력
void printList(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}
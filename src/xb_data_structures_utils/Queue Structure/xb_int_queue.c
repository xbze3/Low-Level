#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int value;
    struct Node *next;
} Node;

Node *xb_createNode(int value)
{
    Node *node = malloc(sizeof(Node));
    if (node == NULL)
        return NULL;

    node->value = value;
    node->next = NULL;
    return node;
}

int xb_enqueue(Node *head, int value)
{
    if (head == NULL)
        return -1;

    Node *tmp = head;
    while (tmp->next)
    {
        tmp = tmp->next;
    }

    Node *new_node = xb_createNode(value);
    if (new_node == NULL)
        return -1;

    tmp->next = new_node;
    return 0;
}

Node *xb_dequeue(Node **head)
{
    if (head == NULL || *head == NULL)
        return NULL;

    Node *tmp = *head;
    *head = (*head)->next;
    return tmp;
}

int xb_printList(Node *head)
{
    if (head == NULL)
        return -1;

    Node *tmp = head;
    while (tmp)
    {
        printf("%d -> ", tmp->value);
        tmp = tmp->next;
    }
    printf("NULL\n");
    return 0;
}

void xb_freeList(Node *head)
{
    Node *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;

        free(tmp);
    }
}

int main()
{
    Node *head = xb_createNode(10);
    if (head == NULL)
    {
        printf("Memory allocation failed\n");
        return -1;
    }

    xb_enqueue(head, 20);
    xb_enqueue(head, 30);
    xb_enqueue(head, 40);
    xb_enqueue(head, 50);
    xb_enqueue(head, 60);
    xb_enqueue(head, 70);

    xb_printList(head);

    Node *dequeuedNode;

    dequeuedNode = xb_dequeue(&head);
    dequeuedNode = xb_dequeue(&head);
    dequeuedNode = xb_dequeue(&head);

    xb_printList(head);

    xb_freeList(head);

    return 0;
}

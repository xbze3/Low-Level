#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    int value;
    struct Node *next;
} Node;

Node *xb_createNode(int value)
{
    Node *node = malloc(sizeof(Node));

    if (node == NULL)
    {
        return NULL;
    }

    node->value = value;
    node->next = NULL;

    return node;
}

int xb_addEnd(Node *head, int value)
{
    if (head == NULL)
    {
        return -1;
    }

    Node *tmp = head;
    while (tmp->next)
    {
        tmp = tmp->next;
    }

    Node *new_node = xb_createNode(value);
    if (new_node == NULL)
    {
        return -1;
    }

    tmp->next = new_node;
    return 0;
}

int xb_printList(Node *head)
{
    if (head == NULL)
    {
        return -1;
    }

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

    xb_addEnd(head, 20);
    xb_addEnd(head, 30);
    xb_addEnd(head, 40);
    xb_addEnd(head, 50);
    xb_addEnd(head, 60);
    xb_addEnd(head, 70);

    xb_printList(head);

    xb_freeList(head);

    return 0;
}

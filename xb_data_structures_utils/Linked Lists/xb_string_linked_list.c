#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char *data;
    struct Node *next;
} Node;

Node *xb_createNode(const char *data)
{
    Node *node = malloc(sizeof(Node));

    if (node == NULL)
        return NULL;

    node->data = malloc((strlen(data) + 1) * sizeof(char));
    if (node->data == NULL)
    {
        free(node);
        return NULL;
    }

    strcpy(node->data, data);
    node->next = NULL;
    return node;
}

int xb_addEnd(Node *head, char *data)
{
    if (data == NULL)
        return -1;

    Node *tmp = head;

    while (tmp->next)
    {
        tmp = tmp->next;
    }

    Node *new_node = xb_createNode(data);

    if (new_node == NULL)
        return -1;

    tmp->next = new_node;

    return 0;
}

int xb_printList(Node *head)
{
    if (head == NULL)
        return -1;

    Node *tmp = head;
    while (tmp)
    {
        printf("%s -> ", tmp->data);
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
        free(tmp->data);
        free(tmp);
    }
}

int main()
{
    Node *head = xb_createNode("Hello");
    if (!head)
    {
        printf("Failed to create head node.\n");
        return -1;
    }

    xb_addEnd(head, "this");
    xb_addEnd(head, "is");
    xb_addEnd(head, "a");
    xb_addEnd(head, "Linked");
    xb_addEnd(head, "List");

    xb_printList(head);

    xb_freeList(head);
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define ALIGNMENT 16
#define ALIGN_SIZE(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

typedef struct Block
{
    int size;
    int free;
    struct Block *next;
    struct Block *prev;
} Block;

static Block *free_list_head = NULL;

void *xb_mloc(int size)
{
    if (size <= 0)
        return NULL;

    size = ALIGN_SIZE(size);

    if (free_list_head == NULL)
    {
        free_list_head = (Block *)sbrk(sizeof(Block));
        if (free_list_head == (void *)-1)
            return NULL;

        free_list_head->size = ALIGN_SIZE(3000);
        free_list_head->free = 1;
        free_list_head->next = NULL;
        free_list_head->prev = NULL;
    }

    Block *tmp = free_list_head;
    Block *bestFit = NULL;

    while (tmp)
    {
        if (tmp->free == 1 && tmp->size >= size)
        {
            if (bestFit == NULL || tmp->size < bestFit->size)
            {
                bestFit = tmp;
            }
        }
        tmp = tmp->next;
    }

    if (bestFit == NULL)
        return NULL;

    bestFit->free = 0;

    if (bestFit->size > size)
    {
        Block *new_block = (Block *)sbrk(sizeof(Block));
        if (new_block == (void *)-1)
            return NULL;

        new_block->size = ALIGN_SIZE(bestFit->size - size);
        new_block->free = 1;
        new_block->next = bestFit->next;
        new_block->prev = bestFit;

        if (new_block->next)
            new_block->next->prev = new_block;

        bestFit->size = size;
        bestFit->next = new_block;

        // printf("Worst Fit Block: %p\n", bestFit);
    }

    return (void *)(bestFit + sizeof(Block));
}

int xb_free(void *rem_block)
{
    if (rem_block == NULL)
    {
        return -1;
    }

    Block *block_to_free = (Block *)rem_block - sizeof(Block);
    block_to_free->free = 1;

    // printf("Free: %p\n", block_to_free);

    // printf("Freeing block at %p with size %d\n", block_to_free, block_to_free->size);

    if (block_to_free->next && block_to_free->next->free == 1)
    {
        // printf("Coalescing with next block at %p\n", block_to_free->next);
        block_to_free->size += block_to_free->next->size;
        Block *to_free = block_to_free->next;
        block_to_free->next = block_to_free->next->next;
        if (block_to_free->next)
        {
            block_to_free->next->prev = block_to_free;
        }
        sbrk(-(sizeof(Block) + to_free->size));
    }

    if (block_to_free->prev && block_to_free->prev->free == 1)
    {
        // printf("Coalescing with previous block at %p\n", block_to_free->prev);
        block_to_free->prev->size += block_to_free->size;
        block_to_free->prev->next = block_to_free->next;
        if (block_to_free->next)
        {
            block_to_free->next->prev = block_to_free->prev;
        }
        sbrk(-(sizeof(Block) + block_to_free->size));
    }

    return 0;
}

int printMem()
{
    Block *tmp = free_list_head;

    if (tmp == NULL)
        return -1;

    while (tmp)
    {
        printf("[Size: %d, Free: %d] -> ", tmp->size, tmp->free);
        tmp = tmp->next;
    }

    printf("NULL\n");

    return 0;
}

int main()
{
    int *test = xb_mloc(sizeof(int));

    *test = 4;

    // printf("Test: %p\n", (Block *)test - sizeof(Block));

    char *test2 = xb_mloc(sizeof(char));
    short *test3 = xb_mloc(sizeof(short));

    printMem();

    xb_free(test);
    xb_free(test2);
    xb_free(test3);

    printf("\n");

    printMem();

    return 0;
}

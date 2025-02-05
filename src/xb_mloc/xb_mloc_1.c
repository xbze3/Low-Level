// This is the first file in my journey of creating my own simple memory allocator

// In this first program I'll only focus on memory block freeing strategies and trying to get
// the "allocator" to work with basic integer values help within a linked list

// After I get this, then I'll move on to actual block of memory

// free == 1 (Block is free)
// free == 0 (Block is NOT free)

#include <stdio.h>
#include <stdlib.h>

typedef struct Block
{
    int size;
    int free;
    struct Block *next;
} Block;

Block *mem_init(int size)
{
    if (size <= 0)
        return NULL;

    Block *block = malloc(sizeof(Block));
    if (block == NULL)
        return NULL;

    block->size = size;
    block->free = 1;
    block->next = NULL;

    return block;
}

Block *xb_mloc(Block *block, int size)
{
    if (size <= 0)
        return NULL;

    Block *tmp = block;
    Block *worstFit = NULL;

    while (tmp)
    {
        if (tmp->free == 1 && tmp->size >= size)
        {
            if (worstFit == NULL || tmp->size > worstFit->size)
            {
                worstFit = tmp;
            }
        }
        tmp = tmp->next;
    }

    if (worstFit == NULL)
        return NULL;

    worstFit->free = 0;

    if (worstFit->size > size)
    {
        Block *new_block = malloc(sizeof(Block));
        if (new_block == NULL)
            return NULL;

        new_block->size = worstFit->size - size;
        new_block->free = 1;
        new_block->next = worstFit->next;

        worstFit->size = size;
        worstFit->next = new_block;
    }

    return worstFit;
}

int xb_free(Block *block, Block *rem_block)
{
    if (block == NULL || rem_block == NULL)
        return -1;

    Block *prev = NULL;
    Block *tmp = block;

    while (tmp && tmp != rem_block)
    {
        prev = tmp;
        tmp = tmp->next;
    }

    if (tmp == NULL)
    {
        printf("Error: Block not found.\n");
        return -1;
    }

    tmp->free = 1;

    if (tmp->next && tmp->next->free == 1)
    {
        tmp->size += tmp->next->size;
        Block *to_free = tmp->next;
        tmp->next = tmp->next->next;
        free(to_free);
    }

    if (prev && prev->free == 1)
    {
        prev->size += tmp->size;
        prev->next = tmp->next;
        free(tmp);
    }

    return 0;
}

int printMem(Block *block)
{
    if (block == NULL)
        return -1;

    Block *tmp = block;

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
    Block *memory = mem_init(3000);

    Block *new1 = xb_mloc(memory, 100);
    Block *new2 = xb_mloc(memory, 300);
    Block *new3 = xb_mloc(memory, 200);
    Block *new4 = xb_mloc(memory, 50);
    Block *new5 = xb_mloc(memory, 70);

    printMem(memory);

    xb_free(memory, new1);
    xb_free(memory, new2);
    xb_free(memory, new3);
    xb_free(memory, new4);
    xb_free(memory, new5);

    printf("\n");

    printMem(memory);

    return 0;
}

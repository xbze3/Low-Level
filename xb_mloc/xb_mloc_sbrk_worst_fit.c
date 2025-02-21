// This is the first file in my journey of creating my own simple memory allocator

// In this first program I'll only focus on memory block freeing strategies and trying to get
// the "allocator" to work with basic integer values help within a linked list

// After I get this, then I'll move on to actual block of memory

// free == 1 (Block is free)
// free == 0 (Block is NOT free)

// Insert after completion
// I decided to go with the worst fit allocation technique. This was in an effort to reduce internal
// fragmentation as much as possible.

// I added functionlity to ensure that memory allocations are alligned to a 16 byte boundary

// I think I got everything working but I'm not entirely sure
// Will do more testing to verify

// From all the basic tests that I've run it seems like it works
// Will do more testing at a later date to confirm

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
        Block *new_block = (Block *)sbrk(sizeof(Block));
        if (new_block == (void *)-1)
            return NULL;

        new_block->size = ALIGN_SIZE(worstFit->size - size);
        new_block->free = 1;
        new_block->next = worstFit->next;
        new_block->prev = worstFit;

        if (new_block->next)
            new_block->next->prev = new_block;

        worstFit->size = size;
        worstFit->next = new_block;

        // printf("Worst Fit Block: %p\n", worstFit);
    }

    return (void *)(worstFit + sizeof(Block));
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

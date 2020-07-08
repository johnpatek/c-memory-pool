#include "memory_pool.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MEMORY_BLOCK_OVERHEAD sizeof(struct memory_block)

struct memory_pool * mp_create(uint32_t size)
{
    struct memory_pool *result;
    struct memory_block first;
    void *buf;

    result = (struct memory_pool*)malloc(sizeof(struct memory_pool));
    if(result != NULL)
    {
        buf = malloc(size);
        if(buf != NULL)
        {
            result->buf = buf;
            result->total_size = size;
            result->free_size = size - MEMORY_BLOCK_OVERHEAD;
            first.prev = NULL;
            first.next = NULL;
            first.is_free = 1;
            first.size = result->free_size;
            memcpy(result->buf,&first,MEMORY_BLOCK_OVERHEAD);
        }
    }
    return result;
}

void mp_destroy(struct memory_pool * const pool)
{
    if(pool != NULL)
    {
        if(pool->buf != NULL)
        {
            free(pool->buf);
        }
        free(pool);
    }
}

void * mp_malloc(struct memory_pool * const pool, uint32_t size)
{
    void *result;
    int error;
    struct memory_block *block;
    uint32_t required_size, remaining_size;

    result = NULL;

    // Make sure we are dealing with a valid pool and valid buffer
    error = (pool == NULL) || (pool->buf == NULL);

    // We should not allow 0 byte allocations
    error = error || (size == 0);

    block = (error)?NULL:pool->buf;

    required_size = size + MEMORY_BLOCK_OVERHEAD;

    // Keep looking until we find a suitable block
    while(!error && (result == NULL) && (block != NULL))
    {
        // if the block is free and large enough, use it.
        if(block->is_free && block->size > required_size)
        {
            pool->free_size -= block->size;
            remaining_size = block->size - required_size;
            block->is_free = 0;
            result = block + MEMORY_BLOCK_OVERHEAD;
        }

        // if enough space is remaining, make another block.
        if(remaining_size > MEMORY_BLOCK_OVERHEAD)
        {
            struct memory_block new_block,*new_block_ptr;
            new_block.is_free = 0;
            new_block.size = remaining_size - MEMORY_BLOCK_OVERHEAD;
            new_block.prev = block;
            new_block_ptr = block + required_size;
            if(block->next != NULL)
            {
                new_block.next->prev = new_block_ptr;
            }
            new_block.next = block->next;
            memcpy(new_block_ptr,&new_block,MEMORY_BLOCK_OVERHEAD);
            block->size = required_size;
            block->next = new_block_ptr;
            pool->free_size += new_block.size;
        }

        block = block->next;
    }

    return result;
}
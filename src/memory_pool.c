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
            // TODO: mark block, set result, and update pool info
        }

        // if enough space is remaining, make another block.
        if(remaining_size > MEMORY_BLOCK_OVERHEAD)
        {
            // TODO: Make a new block and update pool info
        }

        block = block->next;
    }

    return result;
}

void * mp_calloc(struct memory_pool * const pool, uint32_t num, 
    uint32_t size)
{
    void *result;
    uint32_t alloc_size;

    alloc_size = num * size;
    
    result = mp_malloc(pool, alloc_size);
    
    if(result != NULL)
    {
        memset(result,0,alloc_size);
    }

    return result;
}

void * mp_realloc(struct memory_pool * const pool, void * ptr, 
    uint32_t size)
{

}

void mp_free(struct memory_pool * const pool, void * ptr)
{

}

uint32_t mp_free_size(const struct memory_pool * const pool)
{
    return (pool != NULL && pool->buf != NULL)?pool->free_size:0;
}

uint32_t mp_total_size(const struct memory_pool * const pool)
{
    return (pool != NULL && pool->buf != NULL)?pool->total_size:0;
}

uint32_t mp_largest_block_size(const struct memory_pool * const pool)
{
    uint32_t result;
    struct memory_block * block;

    result = 0;

    block = (pool != NULL && pool->buf != NULL)?NULL:pool->buf;

    while(block != NULL)
    {
        if(block->size > result)
        {
            result = block->size;
        }
        block = block->next;
    }
    
    return result;
}

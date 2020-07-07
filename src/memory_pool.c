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

void * mp_malloc(struct memory_pool * const pool, uint32_t size)
{
    int8_t error;
    void *result;
    struct memory_block *block;
    uint32_t required_size, remaining_size;
    
    error = (pool == NULL || pool->buf == NULL);
    
    result = NULL;

    return result;
}
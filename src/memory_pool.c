#include "memory_pool.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MEMORY_BLOCK_OVERHEAD sizeof(memory_block)

memory_pool * mp_create(uint32_t size)
{
    memory_pool *mem_pool = (memory_pool *)malloc(sizeof(memory_pool));
    if(mem_pool == NULL)
    {
        fputs("Out of memory!\n", stderr);
        return mem_pool;
    }
    mem_pool->free_size = size - MEMORY_BLOCK_OVERHEAD;
    mem_pool->total_size = size;
    memory_block mem_header = {NULL, NULL, size - MEMORY_BLOCK_OVERHEAD, 1};
    mem_pool->buf = (void *)malloc(size);
    if(mem_pool->buf == NULL)
    {
        fputs("Out of memory!\n", stderr);
        return mem_pool;
    }
    memcpy(mem_pool->buf, &mem_header, MEMORY_BLOCK_OVERHEAD);

    return mem_pool;
}


void * mp_malloc(memory_pool * const pool, uint32_t size)
{
    uint8_t error;
    void *result = NULL;
    uint32_t remaining_size; 
    
    error = (pool == NULL) || (pool->buf == NULL) || (size == 0);
    
    memory_block * block = (error)?NULL:(memory_block*)pool->buf;

    // Keep iterating until a suitable block is found
    while(!error && block != NULL && result == NULL)
    {
        // Check block to see if it fits requirements
        if(block->is_free && block->size >= size)
        {
            block->is_free = 0;
            remaining_size = block->size - size;
            if(remaining_size > MEMORY_BLOCK_OVERHEAD)
            {
                block->size = size;
                memory_block next_block, *next_ptr;
                next_ptr = (memory_block*)(((uint8_t*)block) + block->size + MEMORY_BLOCK_OVERHEAD);
                next_block.size = remaining_size - MEMORY_BLOCK_OVERHEAD;
                next_block.is_free = 1;
                next_block.prev = block;
                next_block.next = block->next;
                memcpy(next_ptr,&next_block,MEMORY_BLOCK_OVERHEAD);
                if(next_ptr->next != NULL)
                {
                    next_ptr->next->prev = next_ptr;
                }
                block->next = next_ptr;
            }
            result = (uint8_t*)block + MEMORY_BLOCK_OVERHEAD;
        }
        block = block->next;
    }

    return result;
}

void mp_destroy(memory_pool * const pool)
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

void * mp_calloc(memory_pool * const pool, uint32_t num, 
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

void * mp_realloc(memory_pool * const pool, void * ptr, 
    uint32_t size)
{
    return NULL;
}

void mp_free(memory_pool * const pool, void * ptr)
{
    memory_block *current_header, *prev_header, *next_header;
    uint8_t error, merge_prev, merge_next;
    uint32_t size_freed;

    error = (pool == NULL) || (pool->buf == NULL) || (ptr ==  NULL);

    current_header = (error)?NULL:(memory_block*)((uint8_t*)ptr - MEMORY_BLOCK_OVERHEAD);

    if(!error && current_header->is_free == 0)
    {
        prev_header = current_header->prev;
        next_header = current_header->next;
        
        // Check if prev is a free block
        merge_prev = (prev_header != NULL) && (prev_header->is_free == 1);
        
        // Check if next is a free block
        merge_next = (next_header != NULL) && (next_header->is_free == 1);
        
        // 
        if(merge_prev == 1 && merge_next == 1)
        {
            current_header->is_free = 1;
            size_freed = MEMORY_BLOCK_OVERHEAD + current_header->size + MEMORY_BLOCK_OVERHEAD;
        }
        else if(merge_prev == 1)
        {
            current_header->is_free = 1;
            size_freed = MEMORY_BLOCK_OVERHEAD + current_header->size;
        }
        else if(merge_next == 1)
        {
            current_header->is_free = 1;
            size_freed = current_header->size + MEMORY_BLOCK_OVERHEAD;
        }
        else
        {
            current_header->is_free = 1;
            size_freed = current_header->size; 
        }

        pool->free_size += size_freed;
    }
}

uint32_t mp_free_size(const memory_pool * const pool)
{
    return (pool != NULL && pool->buf != NULL)?pool->free_size:0;
}

uint32_t mp_total_size(const memory_pool * const pool)
{
    return (pool != NULL && pool->buf != NULL)?pool->total_size:0;
}

uint32_t mp_largest_block_size(const memory_pool * const pool)
{
    uint32_t result;
    memory_block * block;

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

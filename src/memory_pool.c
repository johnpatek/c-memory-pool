#include "memory_pool.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

memory_pool * mp_create(uint32_t size)
{
    memory_pool *mem_pool = (memory_pool *)malloc(sizeof(memory_pool));
    if(mem_pool == NULL)
    {
        fputs("Out of memory!\n", stderr);
        return mem_pool;
    }
    mem_pool->free_size = size - sizeof(memory_block);
    mem_pool->total_size = size;
    memory_block mem_header = {NULL, NULL, size, 1};
    mem_pool->buf = (void *)malloc(size);
    if(mem_pool->buf == NULL)
    {
        fputs("Out of memory!\n", stderr);
        return mem_pool;
    }
    memcpy(mem_pool->buf, &mem_header, sizeof(memory_block));

    return mem_pool;
}


void * mp_malloc(memory_pool * const pool, uint32_t size)
{
    void *result = NULL;
    uint32_t required_size = size + sizeof(memory_block);
    uint32_t remaining_size; 

    if(pool != NULL && pool->buf != NULL)
    {
        memory_block *block = (memory_block *)pool->buf;
        /*
	  As long as there is nothing assigned to result and you have blocks left, keep iterating
	 */
        while(result == NULL && block != NULL)
	{
            if(block->is_free && block->size >= required_size && pool->free_size > required_size)
	    {	
	        remaining_size = block->size - required_size;
                block->is_free = 0;
                block->size = size;
                pool->free_size -= required_size;
                result = block + sizeof(memory_block);
		              
                if(remaining_size > sizeof(memory_block))
		{
                    memory_block mem_header, *new_header;
                    mem_header.prev = block;
                    mem_header.next = block->next;
		    if(block->next != NULL)
		    {
                        mem_header.next->prev = new_header;
		    }

                    mem_header.size = remaining_size - sizeof(memory_block);
                    mem_header.is_free = 1;
              	    new_header = block + required_size;   
                    block->next = new_header;
                    pool->free_size -= sizeof(memory_block);
                    memcpy(new_header, &mem_header, sizeof(memory_block));
                }
	    }
            block = block->next; 
	}
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

}

void mp_free(memory_pool * const pool, void * ptr)
{
    memory_block *current_header, *prev_header, *next_header;
    current_header = (memory_block*)(ptr - sizeof(memory_block));

    if(current_header->is_free)
    {
        return;
    }

    next_header = current_header->next;
    prev_header = current_header->prev;
    current_header->is_free = 1;
    pool->free_size += current_header->size;
 
    if(next_header->is_free)
    {
        pool->free_size += sizeof(memory_block);
	
        current_header->next = next_header->next;
	current_header->size += sizeof(memory_block) + next_header->size;
	if(prev_header->is_free)
	{
            prev_header->next = current_header->next;
            prev_header->size += current_header->size + sizeof(memory_block);
	    pool->free_size += sizeof(memory_block);
	}
    }
    else if(prev_header->is_free)
    {
        prev_header->next = current_header->next;
        pool->free_size += sizeof(memory_block);
	prev_header->size += current_header->size + sizeof(memory_block);
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

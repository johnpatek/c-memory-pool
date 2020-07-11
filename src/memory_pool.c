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

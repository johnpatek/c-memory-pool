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
    mem_pool->free_size = size;
    mem_pool->total_size = size;
    memory_block mem_header = {NULL, NULL, size, 1};
    mem_pool->buf = (void *)malloc(sizeof(size));
    memcpy(mem_pool->buf, &mem_header, sizeof(memory_block));

    return mem_pool;
}

void * mp_malloc(memory_pool * const pool, uint32_t size)
{
    printf("is free: %d\n", ((memory_block*)(pool->buf))->is_free);
    if(((memory_block*)(pool->buf))->is_free)
    {
        ((memory_block*)(pool->buf))->is_free = 0;
        ((memory_block*)(pool->buf))->size -= size;
        // create new header
        memory_block mem_header = {NULL, NULL, size, 1};
        // copy new header to new block
        // first check the bounds are still correct 
	if(pool->total_size > sizeof(memory_block) + size)
        {
	    printf("is able to create new block\n");
            memcpy((uint8_t*)pool->buf + size, &mem_header, sizeof(memory_block));
	    // allocated block's 'next' should  point to the newly created block
            ((memory_block*)(pool->buf))->next = (uint8_t*)pool->buf + size;
	    //printf("%ld \n \n",  ((memory_block*)(((memory_block*)(pool->buf))->next))->next); 
	}
	return (pool->buf + sizeof(memory_block));
    } 
    return NULL;
}

void mp_mem_dump(memory_pool * pool)
{
    uint8_t *blk = (uint8_t *)pool->buf;
    do{
        for(int i = 0; i < 
           ((memory_block*)(pool->buf))->size + sizeof(memory_block); ++i, ++blk)
        {
            if(i%24 == 0)
	    {
                printf("\n");
	    }
	    printf("%02x ", *blk);
        }
        blk = (uint8_t*)((memory_block*)(pool->buf))->next;
	// fix me, i posit that in mp_malloc, pointer arithmetic is off
    }while(0);
    blk =  (uint8_t*)((memory_block*)(pool->buf))->next; 
    printf("%ld\n", blk);
}

int main(void)
{
    memory_pool *mem_pool = mp_create(sizeof(uint8_t)*0xFF);
    mp_mem_dump(mem_pool);
    char *buff = mp_malloc(mem_pool, 10);
    char test_str[] = "hey there!\n";
    memcpy(buff, test_str, sizeof(test_str) + 1);
    printf("%s\n", buff);
    memory_block blk = {(struct memory_block*)0xDEADBEEF, (struct memory_block*)0xDEADBEEF, 0xFF, 0x01};
    uint8_t *ptr = (uint8_t*)&blk;
    for(int i = 0; i < sizeof(memory_block); ++i)
    {
        printf("%02x:", *ptr);
	++ptr;
    }
    mp_mem_dump(mem_pool);
    printf("size of %d %d %d %d, entire thing: %d\n", sizeof(memory_block *),  sizeof(memory_block *)
		                                    , sizeof(uint32_t), 1, sizeof(memory_block));
    return 0;
}

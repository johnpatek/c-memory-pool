#include "memory_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dump_pool(const  memory_pool * const pool);

static void dump_block(const memory_block * const block);

static void dump_header(const memory_block * const block);

static void dump_payload(const memory_block * const block);

static void mp_simple_destroy(memory_pool * const pool)
{
    free(pool->buf);
    free(pool);
}

int main(int argc, char ** argv)
{
    memory_pool * pool;
    void *buf1, *buf2;

    pool = mp_create(1024);
    
    memset((uint8_t*)pool->buf + sizeof(memory_block),0,pool->free_size);
   
    dump_pool(pool);
    
    buf1 = mp_malloc(pool,128);

    buf2 = mp_malloc(pool,256);

    dump_pool(pool);

    mp_destroy(pool);
    
    return 0;
}

static void dump_pool(const memory_pool * const pool)
{
    const memory_block * block;

    puts("Dumping pool...");
    printf("Pool buf: %p\n",pool->buf);
    printf("Free size: %u\n",pool->free_size);
    printf("Total size: %u\n",pool->total_size);

    block = (memory_block*)pool->buf;    
    while(block != NULL)
    {
        dump_block(block);
        block = block->next;
    }
    puts("");
}

static void dump_block(const memory_block * const block)
{
    printf("block @%p:\n",block);
    dump_header(block);
    dump_payload(block);
}

static void dump_header(const memory_block * const block)
{
    printf("prev: %p\n",block->prev);
    printf("next: %p\n",block->next);
    printf("size: %u\n",block->size);
    printf("is_free: %hhu\n",block->is_free);
}

static void dump_payload(const memory_block * const block)
{
    uint32_t off;
    const uint8_t * const buf = (const uint8_t* const)block + sizeof(memory_block);
    printf("data @%p:",buf);
    for(off = 0; off < block->size; off++)
    {
        printf("%c%02x",(off % 24)?':':'\n',*(buf + off));
    }
    puts("");
}

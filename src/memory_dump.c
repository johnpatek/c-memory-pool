#include <memory_pool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dump_pool(const struct memory_pool * const pool);

static void dump_block(const struct memory_block * const block);

static void dump_header(const struct memory_block * const block);

static void dump_payload(const struct memory_block * const block);

static void mp_simple_destroy(struct memory_pool * const pool)
{
    free(pool->buf);
    free(pool);
}

int main(int argc, char ** argv)
{
    struct memory_pool * pool;
    void * buf;

    pool = mp_create(256);
    
    memset((uint8_t*)pool->buf + sizeof(struct memory_block),0,pool->free_size);
   
    dump_pool(pool);
    
    buf = mp_malloc(pool,128);
    
    dump_pool(pool);

    mp_simple_destroy(pool);
    
    return 0;
}

static void dump_pool(const struct memory_pool * const pool)
{
    const struct memory_block * block;

    puts("Dumping pool...");
    printf("Pool buf: %X\n",pool->buf);
    printf("Free size: %u\n",pool->free_size);
    printf("Total size: %u\n",pool->total_size);

    block = (struct memory_block*)pool->buf;    
    while(block != NULL)
    {
        dump_block(block);
        block = block->next;
    }
    puts("");
}

static void dump_block(const struct memory_block * const block)
{
    printf("block @%X:\n",block);
    dump_header(block);
    dump_payload(block);
}

static void dump_header(const struct memory_block * const block)
{
    printf("prev: %X\n",block->prev);
    printf("next: %X\n",block->next);
    printf("size: %u\n",block->size);
    printf("is_free: %hhu\n",block->is_free);
}

static void dump_payload(const struct memory_block * const block)
{
    uint32_t off;
    const uint8_t * const buf = (const uint8_t* const)block + sizeof(struct memory_block);
    printf("data @%X:",buf);
    for(off = 0; off < block->size; off++)
    {
        printf("%c%02X",(off % 24)?':':'\n',*(buf + off));
    }
    puts("");
}
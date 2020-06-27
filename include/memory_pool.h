#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H
#include <stdint.h>

struct memory_block
{
    struct memory_block * prev;
    struct memory_block * next;
    uint32_t size;
    uint8_t is_free;
};

struct memory_pool
{
    struct memory_block * head;
    uint32_t free_size;
    uint32_t total_size;
};


struct memory_pool * mp_create(uint32_t size);

void mp_destroy(struct memory_pool * const pool);

void * mp_malloc(struct memory_pool * const pool, uint32_t size);

void * mp_calloc(struct memory_pool * const pool, uint32_t size);

void * mp_realloc(struct memory_pool * const pool, uint32_t size);

void mp_free(void * buf);

uint32_t mp_free_size(const struct memory_pool * const pool);

uint32_t mp_total_size(const struct memory_pool * const pool);

uint32_t mp_largest_block_size(const struct memory_pool * const pool);

#endif
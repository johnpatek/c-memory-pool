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

/**
 * @brief Create a new memory pool
 * @param size The size of the memory block to be managed by the pool
 * @return A newly created memory pool for managing the block of
 * memory 
 */
struct memory_pool * mp_create(uint32_t size);

/**
 * @brief Destroy the memory pool and free the managed memory
 * @param pool A pointer to the pool object to be destroyed
 */
void mp_destroy(struct memory_pool * const pool);

/**
 * @brief Allocate a block of memory from the pool
 * @param pool A valid pointer to a memory pool
 * @param size The desired size of the allocated block of memory
 * @return A pointer to the beginning of the newly allocated block,
 * NULL if the size exceeds either the amount of free space or the
 * size of the largest free block
 */
void * mp_malloc(struct memory_pool * const pool, uint32_t size);

/**
 * @brief Allocates memory for an array of num objects of size and 
 * initializes all bytes in the allocated storage to zero.
 * @param 
 * @param 
 * @param 
 * @return 
 */
void * mp_calloc(struct memory_pool * const pool, uint32_t num, 
    uint32_t size);

void * mp_realloc(struct memory_pool * const pool, uint32_t size);

void mp_free(void * buf);

uint32_t mp_free_size(const struct memory_pool * const pool);

uint32_t mp_total_size(const struct memory_pool * const pool);

uint32_t mp_largest_block_size(const struct memory_pool * const pool);

#endif
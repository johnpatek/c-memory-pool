#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H
/** @file */
#include <stdint.h>

typedef struct mem_block
{
    struct mem_block * prev;
    struct mem_block * next;
    uint32_t size;
    uint8_t is_free;
}memory_block;

typedef struct mem_pool
{
    void * buf;
    uint32_t free_size;
    uint32_t total_size;
}memory_pool;

/**
 * @brief Create a new memory pool
 * @param size The size of the memory block to be managed by the pool
 * @return A newly created memory pool for managing the block of
 * memory 
 */
memory_pool * mp_create(uint32_t size);

/**
 * @brief Destroy the memory pool and free the managed memory
 * @param pool A pointer to the pool object to be destroyed
 */
void mp_destroy(memory_pool * const pool);

/**
 * @brief Allocate a block of uninitialized memory from the pool.
 * @param pool pointer to a valid memory pool
 * @param size number of bytes to allocate 
 * @return A pointer to the beginning of the newly allocated block,
 * NULL if the size exceeds either the amount of free space or the
 * size of the largest free block.
 */
void * mp_malloc(memory_pool * const pool, uint32_t size);

/**
 * @brief Allocates memory for an array of num objects of size and 
 * initializes all bytes in the allocated storage to zero.
 * @param pool pointer to a valid memory pool
 * @param num number of objects
 * @param size size of each object 
 * @return A pointer to the beginning of the newly allocated block,
 * NULL if the size exceeds either the amount of free space or the
 * size of the largest free block. 
 */
void * mp_calloc(memory_pool * const pool, uint32_t num, 
    uint32_t size);

/**
 * @brief Reallocates the given area of memory. Will relocated to
 * a new address if cannot be sufficiently expanded.
 * @param pool pointer to a valid memory pool
 * @param ptr  pointer to the memory area to be reallocated 
 * @param size new size of the buffer in bytes
 * @return A pointer to the beginning of the resized block,
 * NULL if the block cannot be resized and a new block of sufficient
 * size cannot be found.
 */
void * mp_realloc(memory_pool * const pool, void * ptr, 
    uint32_t size);

/**
 * @brief Deallocates the space previously allocated
 * @param pool pointer to a valid memory pool
 * @param ptr  pointer to the memory area to be deallocated
 */
void mp_free( memory_pool * const pool, void * ptr);

/**
 * @brief Get the amount of free space for a given pool
 * @param pool pointer to a valid memory pool
 * @return The free space available in number of bytes.
 */
uint32_t mp_free_size(const memory_pool * const pool);

/**
 * @brief Get the total size of the memory managed by the pool
 * @param pool pointer to a valid memory pool
 * @return The total pool size in number of bytes.
 */
uint32_t mp_total_size(const memory_pool * const pool);

/**
 * @brief Get the size of the largest free block in the memory pool
 * @param pool pointer to a valid memory pool
 * @return The size of the largest block available in number of bytes.
 */
uint32_t mp_largest_block_size(const memory_pool * const pool);


void mp_aquire(memory_pool * const pool, void * const buf, 
    uint32_t len);

void mp_release(memory_pool * const pool, void ** buf, 
    uint32_t * const len);

#endif

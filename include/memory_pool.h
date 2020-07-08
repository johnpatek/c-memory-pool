#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H
/** @file */
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
    void * buf;
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
 * @brief Allocate a block of uninitialized memory from the pool.
 * @param pool pointer to a valid memory pool
 * @param size number of bytes to allocate 
 * @return A pointer to the beginning of the newly allocated block,
 * NULL if the size exceeds either the amount of free space or the
 * size of the largest free block.
 */
void * mp_malloc(struct memory_pool * const pool, uint32_t size);

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
void * mp_calloc(struct memory_pool * const pool, uint32_t num, 
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
void * mp_realloc(struct memory_pool * const pool, void * ptr, 
    uint32_t size);

/**
 * @brief Deallocates the space previously allocated
 * @param pool pointer to a valid memory pool
 * @param ptr  pointer to the memory area to be deallocated
 */
void mp_free(struct memory_pool * const pool, void * ptr);

/**
 * @brief Get the amount of free space for a given pool
 * @param pool pointer to a valid memory pool
 * @return The free space available in number of bytes.
 */
uint32_t mp_free_size(const struct memory_pool * const pool);

/**
 * @brief Get the total size of the memory managed by the pool
 * @param pool pointer to a valid memory pool
 * @return The total pool size in number of bytes.
 */
uint32_t mp_total_size(const struct memory_pool * const pool);

/**
 * @brief Get the size of the largest free block in the memory pool
 * @param pool pointer to a valid memory pool
 * @return The size of the largest block available in number of bytes.
 */
uint32_t mp_largest_block_size(const struct memory_pool * const pool);


#endif

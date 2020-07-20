#include "memory_pool.h"
#include <stdlib.h>

static int mp_create_test();

static int mp_destroy_test();

static int mp_malloc_test();

static int mp_calloc_test();

static int mp_realloc_test();

static int mp_free_test();

static int mp_free_size_test();

static int mp_total_size_test();

static int mp_largest_block_size_test();

int main(const int argc, const char ** argv)
{
    int error;
    #if defined(MP_CREATE_TEST)
        error = mp_create_test();
    #elif defined(MP_DESTROY_TEST)
        error = mp_destroy_test();
    #elif defined (MP_MALLOC_TEST)
        error = mp_malloc_test();
    #elif defined (MP_CALLOC_TEST)
        error = mp_calloc_test();
    #elif defined (MP_REALLOC_TEST)
        error = mp_realloc_test();
    #elif defined (MP_FREE_TEST)
        error = mp_free_test();
    #elif defined (MP_FREE_SIZE_TEST)
        error = mp_free_size_test();
    #elif defined (MP_TOTAL_SIZE_TEST)
        error = mp_total_size_test();
    #elif defined (MP_LARGEST_BLOCK_SIZE_TEST)
        error = mp_largest_block_size_test();
    #else
        // Not a vald test case, should be unreachable.
        error = -1;
    #endif
    return error;
}

// Create a pool of 1KB
static int mp_create_test()
{
    int error;
    memory_pool * pool;
    pool = mp_create(1024);
    error = (pool == NULL || pool->buf == NULL);
    mp_destroy(pool);
    return error;
}

// Just don't segfault. Might want to do a detailed check with Valgrind.
static int mp_destroy_test()
{
    memory_pool * pool;
    pool = mp_create(1024);
    mp_destroy(pool);
    return 0;
}

// Make sure the pool can allocate some memory
static int mp_malloc_test()
{
    int error;
    memory_pool * pool;
    void *buf;
    pool = mp_create(1024);
    buf = mp_malloc(pool,100);
    error = (buf == NULL);
    mp_destroy(pool);
    return error;
}

// Make sure the block is count * size of all zeroes
static int mp_calloc_test()
{
    int error,offset;
    memory_pool * pool;
    void *buf;
    pool = mp_create(1024);
    buf = mp_calloc(pool,100,sizeof(int));
    error = (buf == NULL);
    offset = 0;
    // check each byte to make sure they are zeroed out
    while(!error && offset < (100 * sizeof(int)))
    {
        error = (*((uint8_t*)buf + offset) != 0); 
        offset++;
    }
    mp_destroy(pool);
    return 0;
}

// Test realloc from NULL and from existing buffer
static int mp_realloc_test()
{
    int error;
    memory_pool * pool;
    void* test_buf;
    pool = mp_create(1024);
    uint32_t initial_free_size = pool->free_size;
    test_buf = mp_malloc(pool,100);
    error = (mp_realloc(NULL,test_buf, 200) != NULL);
    test_buf = mp_realloc(pool,test_buf,200);
    error = (pool->free_size) != (initial_free_size - 100);
    mp_destroy(pool);
    return error;
}

// Make sure free space is same after a malloc and free
static int mp_free_test()
{
    int error, initial_free_size;
    memory_pool * pool;
    void *buf;
    pool = mp_create(1024);
    initial_free_size = mp_free_size(pool);
    buf = mp_malloc(pool,100);
    error = (buf == NULL);
    mp_free(pool,buf);
    error = error || (initial_free_size != mp_free_size(pool));
    mp_destroy(pool);
    return error;
}

// Make sure pool is reporting correct amount of free space
static int mp_free_size_test()
{
    int error, initial_free_size,delta;
    memory_pool * pool;
    void* buf;
    delta = 10;
    pool = mp_create(1024);
    buf = mp_realloc(pool,NULL,100);
    initial_free_size = mp_free_size(pool);
    buf = mp_realloc(pool,buf,100 + delta);
    error = mp_free_size(pool) != (initial_free_size - delta);
    mp_destroy(pool);
    return error;
}

// Pool should correctly report how much memory is being managed
static int mp_total_size_test()
{
    int error;
    memory_pool * pool;
    pool = mp_create(1024);
    error = (mp_total_size(pool) != 1024);
    mp_destroy(pool);
    return error;
}

// Search and find the biggest free block
static int mp_largest_block_size_test()
{
    int error, expected_size;
    memory_pool * pool;
    void *buf1,*buf2;
    pool = mp_create(1024);
    buf1 = mp_malloc(pool,10);
    buf2 = mp_malloc(pool,20);
    expected_size = mp_free_size(pool);
    mp_free(pool,buf1);
    error = (expected_size != mp_largest_block_size(pool));
    mp_destroy(pool);
    return 0;
}

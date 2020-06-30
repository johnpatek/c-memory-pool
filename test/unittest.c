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
    int result;
    #if defined(MP_CREATE_TEST)
        result = mp_create_test();
    #elif defined(MP_DESTROY_TEST)
        result = mp_destroy_test();
    #elif defined (MP_MALLOC_TEST)
        result = mp_malloc_test();
    #elif defined (MP_CALLOC_TEST)
        result = mp_calloc_test();
    #elif defined (MP_REALLOC_TEST)
        result = mp_realloc_test();
    #elif defined (MP_FREE_TEST)
        result = mp_free_test();
    #elif defined (MP_FREE_SIZE_TEST)
        result = mp_free_size_test();
    #elif defined (MP_TOTAL_SIZE_TEST)
        result = mp_total_size_test();
    #elif defined (MP_LARGEST_BLOCK_SIZE_TEST)
        result = mp_largest_block_size_test();
    #else
    // Not a vald test case, should be unreachable.
        result = -1;
    #endif

    return result;
}

static int mp_create_test()
{
    return 0;
}

static int mp_destroy_test()
{
    return 0;
}

static int mp_malloc_test()
{
    return 0;
}

static int mp_calloc_test()
{
    return 0;
}

static int mp_realloc_test()
{
    return 0;
}

static int mp_free_test()
{
    return 0;
}

static int mp_free_size_test()
{
    return 0;
}

static int mp_total_size_test()
{
    return 0;
}

static int mp_largest_block_size_test()
{
    return 0;
}
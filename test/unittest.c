#include "memory_pool.h"
#include <stdlib.h>

int create_pool_test()
{
    int result;
    struct memory_pool *pool;
    pool = mp_create(1024);
    result = (pool == NULL);
    mp_destroy(pool);
    return result;
}

int destroy_pool_test()
{

}

int main(const int argc, const char ** argv)
{
    int result;
    #if defined(MP_CREATE_TEST)
        result = create_pool_test();
    #elif defined(MP_DESTROY_TEST)
        result = destroy_pool_test();
    #elif defined (MP_MALLOC_TEST)

    #else
        result = 0;
    #endif
    return 0;
}
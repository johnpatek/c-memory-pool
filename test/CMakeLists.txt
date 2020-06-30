add_executable(mp_create unittest.c)
target_include_directories(mp_create PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(mp_create memorypool)

add_executable(mp_destroy unittest.c)
target_include_directories(mp_destroy PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(mp_destroy memorypool)

target_compile_definitions(mp_create PRIVATE MP_CREATE_POOL_TEST)

add_test(mp_create_test unittest)
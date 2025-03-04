
add_library(surt STATIC
    surt_bubble.c
    surt_select.c
    surt_insert.c
    surt_merge.c
    surt_quick.c
    surt_heap.c
)

target_compile_definitions(
    surt PRIVATE
    $<$<CONFIG:Debug>:DEBUG_MODE>
)

target_include_directories(surt PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
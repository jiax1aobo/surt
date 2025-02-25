
add_library(surt STATIC
    surt_bubble.c
    surt_select.c
    surt_insert.c
)

target_include_directories(surt PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
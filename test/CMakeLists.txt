file(GLOB TEST_SRC "*.c")

foreach(test_src ${TEST_SRC})
    get_filename_component(test_name ${test_src} NAME_WE)

    add_executable(${test_name} ${test_src})

    target_compile_definitions(
        ${test_name} PRIVATE
        $<$<CONFIG:Debug>:DEBUG_MODE>
    )

    target_link_libraries(${test_name} PRIVATE surt)
endforeach(test_src ${TEST_SRC})

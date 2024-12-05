macro(compile_output_test name)
    set(additional_libs ${ARGN})
    add_executable(${name} "${name}.cpp")
    target_link_libraries(${name} gtest::gtest ${additional_libs})
    target_include_directories(${name} PRIVATE ${CMAKE_SOURCE_DIR})

    add_test(NAME ${name} COMMAND ${name})
endmacro(compile_output_test)

compile_output_test(deferredTest)
compile_output_test(storageTest)

cmake_minimum_required (VERSION 3.2)
project (libcpputil-tests)

set(linux-sources
    tuple_for_each.cxx
    tuple_at.cxx
    call_seq.cxx
    sizeof_tuple.cxx
    marshall.cxx
    cstring_ref.cxx
    common_headers_compilation.cxx
    uninitialized_array.cxx
    tuple.cxx
    stack.cxx
    bitmap_allocator.cxx
    queue.cxx
    format.cxx
    array.cxx
    for_each_param.cxx
    bitview.cxx
    int_type.cxx
    named_type.cxx
    stack_allocator.cxx
    static_config.cxx
)

set(cpp17-linux-sources
    string_tests.cxx
    getline_tests.cxx
)

set(requires-exceptions-sources
    curl.cxx
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
list(APPEND test-libs cpptoolbox)

set(test-sources ${linux-sources})

if (TOOLBOX_HAS_EXCEPTIONS)
    set(test-source "${test-sources} ${requires-exceptions-sources}")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
endif ()

if (TOOLBOX_CXX_STANDARD GREATER "16")
    list(APPEND test-sources ${cpp17-linux-sources})
endif ()

if (TOOLBOX_USE_FMT)
    list(APPEND test-libs fmt)
endif ()

add_executable(tests ${test-sources} main.cxx)
target_link_libraries(tests ${test-libs})

include(CTest)
add_test(
    NAME "unit tests"
    COMMAND tests
)

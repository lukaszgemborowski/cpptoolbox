cmake_minimum_required (VERSION 3.2)
project (libcpputil)
set (CMAKE_CXX_STANDARD 14)

option (CONFIG_TESTS "build unit tests" OFF)
option (CONFIG_EXAMPLES "build examples" OFF)

set(include-dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(test-dir ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(test-sources
	${test-dir}/tuple_for_each.cxx
	${test-dir}/tuple_at.cxx
	${test-dir}/call_seq.cxx
	${test-dir}/curl.cxx
	${test-dir}/make_ref_tuple.cxx
	${test-dir}/argv.cxx
	${test-dir}/for_each_param.cxx
	${test-dir}/stack_allocator.cxx
	${test-dir}/bitview.cxx
	${test-dir}/int_type.cxx
	${test-dir}/sizeof_tuple.cxx
	${test-dir}/marshall.cxx
)

add_library (cpptoolbox INTERFACE)
target_include_directories (cpptoolbox INTERFACE ${include-dir})

if (CONFIG_TESTS)
    add_executable(tests ${test-sources})
    target_include_directories(tests PRIVATE ${tests})
    target_link_libraries(tests curl cpptoolbox)

    include(CTest)
    add_test(
	NAME "unit tests"
	COMMAND tests)
endif (CONFIG_TESTS)

if (CONFIG_EXAMPLES)
    add_subdirectory(examples)
endif (CONFIG_EXAMPLES)

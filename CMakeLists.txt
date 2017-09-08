cmake_minimum_required (VERSION 3.2)
project (libcpputil)
set (CMAKE_CXX_STANDARD 14)

set(include-dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(test-dir ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(test-sources
	${test-dir}/tuple_for_each.cxx
	${test-dir}/tuple_at.cxx
	${test-dir}/call_seq.cxx
	${test-dir}/curl.cxx
	${test-dir}/make_ref_tuple.cxx
	${test-dir}/argv.cxx
)

add_executable(tests ${test-sources})
include_directories(${include-dir})
target_include_directories(tests PRIVATE ${tests})
target_link_libraries(tests curl)

include(CTest)
add_test(
	NAME "unit tests"
	COMMAND tests)

add_subdirectory(examples)

cmake_minimum_required (VERSION 3.2)
project (libcpputil)
set (CMAKE_CXX_STANDARD 14)

set(include-dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(test-dir ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(test-sources
	${test-dir}/tuple_for_each.cxx
)

add_executable(tests ${test-sources})
target_include_directories(tests PRIVATE ${include-dir} ${tests})

include(CTest)
add_test(
	NAME "unit tests"
	COMMAND tests)

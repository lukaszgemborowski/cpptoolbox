cmake_minimum_required (VERSION 3.2)
project (libcpputil-tests)

set(linux-sources
	tuple_for_each.cxx
	tuple_at.cxx
	call_seq.cxx
	curl.cxx
	make_ref_tuple.cxx
	argv.cxx
	for_each_param.cxx
	stack_allocator.cxx
	bitview.cxx
	int_type.cxx
	sizeof_tuple.cxx
	marshall.cxx
	named_type.cxx
	array.cxx
)

set(osless-sources
	uninitialized_array.cxx
)

if (TOOLBOX_HAS_STDLIB)
	set(test-sources ${osless-sources} ${linux-sources})
else ()
	set(CMAKE_CXX_FLAGS "-nostdinc++")
	set(test-sources ${osless-sources})
endif ()

add_executable(tests ${test-sources} main.cxx)
target_link_libraries(tests curl cpptoolbox)

include(CTest)
add_test(
    NAME "unit tests"
    COMMAND tests
)
cmake_minimum_required (VERSION 3.2)
project (libcpputil-tests)

set(test-sources
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
)

add_executable(tests ${test-sources})
target_link_libraries(tests curl cpptoolbox)

include(CTest)
add_test(
    NAME "unit tests"
    COMMAND tests
)
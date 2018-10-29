#include <toolbox/testing/test.h>
#include <toolbox/cpp/call_seq.hpp>

TEST_CASE(call_seq_simple_use_case)
{
	// create sequence of lambda calls
	auto seq = toolbox::cpp::make_call_seq(
		[](auto &value) {
			value += 2;
		},
		[](auto &value) {
			value *= 2;
		}
	);

	auto value = 2;
	seq(value);

	CHECK(value == (2+2)*2);
}

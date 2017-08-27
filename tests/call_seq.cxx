#include "catch.hpp"
#include <cpp/call_seq.hpp>

TEST_CASE("Simple use case", "[call_seq]")
{
	// create sequence of lambda calls
	auto seq = cpp::make_call_seq(
		[](auto &value) {
			value += 2;
		},
		[](auto &value) {
			value *= 2;
		}
	);

	auto value = 2;
	seq(value);

	REQUIRE(value == (2+2)*2);
}

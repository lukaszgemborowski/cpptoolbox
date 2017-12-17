#include "catch.hpp"
#include <toolbox/marshall/marshall.hpp>
#include <sstream>


struct fixed_size : public toolbox::marshall::record<
		std::uint32_t,
		std::uint32_t,
		std::uint8_t
	>
{
	fixed_size() : record(
		{"seqNumber", "Sequence number of the message"},
		{"payload", "Value to be written"},
		{"crc", "Error checking value"}
	)
	{}
};

struct compound : public toolbox::marshall::record<
		std::int32_t,
		fixed_size,
		std::uint16_t
	>
{
	compound() : record(
		{"prefix", "a prefix number"},
		{"fixed", "a fixed size object"},
		{"postfix", "a 16 bit postix value"}
	)
	{}
};

TEST_CASE("Visiting", "")
{
	compound fs, fd;
	std::stringstream buffer;

	fs.get<0>() = 1;
	fs.get<1>().get<0>() = 2;
	fs.get<1>().get<1>() = 3;
	fs.get<1>().get<2>() = 4;
	fs.get<2>() = 5;

	toolbox::marshall::serialize(buffer, fs);
	toolbox::marshall::deserialize(buffer, fd);

	REQUIRE(fs.get<0>() == fd.get<0>());
	REQUIRE(fs.get<1>().get<0>() == fd.get<1>().get<0>());
	REQUIRE(fs.get<1>().get<1>() == fd.get<1>().get<1>());
	REQUIRE(fs.get<1>().get<2>() == fd.get<1>().get<2>());
	REQUIRE(fs.get<2>() == fd.get<2>());
}

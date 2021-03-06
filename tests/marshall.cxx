#include <toolbox/testing/test.hpp>
#include <toolbox/marshall/marshall.hpp>
#include <sstream>

struct fixed_size : public toolbox::marshall::record<
		std::uint32_t,
		std::uint32_t,
		std::uint8_t,
		toolbox::marshall::dynamic_array<int, int>
	>
{
	fixed_size() : record(
		{"seqNumber", "Sequence number of the message"},
		{"payload", "Value to be written"},
		{"crc", "Error checking value"},
		{"arr", "Array of ints"}
	)
	{}
};

struct compound : public toolbox::marshall::record<
		std::int32_t,
		fixed_size,
		std::uint16_t,
		char [2]
	>
{
	compound() : record(
		{"prefix", "a prefix number"},
		{"fixed", "a fixed size object"},
		{"postfix", "a 16 bit postix value"},
		{"two_chars", "two characters"}
	)
	{}
};

TEST_CASE(marshall_serialize_and_deserialize)
{
	compound fs, fd;

	fs.get<0>() = 1;
	fs.get<1>().get<0>() = 2;
	fs.get<1>().get<1>() = 3;
	fs.get<1>().get<2>() = 4;
	fs.get<1>().get<3>().push_back(42);
	fs.get<2>() = 5;
	fs.get<3>()[0] = 'A';
	fs.get<3>()[1] = 'B';

	{
		std::stringstream buffer;
		toolbox::marshall::serialize(buffer, fs);
		toolbox::marshall::deserialize(buffer, fd);
	}

	CHECK(fs.get<0>() == fd.get<0>());
	CHECK(fs.get<1>().get<0>() == fd.get<1>().get<0>());
	CHECK(fs.get<1>().get<1>() == fd.get<1>().get<1>());
	CHECK(fs.get<1>().get<2>() == fd.get<1>().get<2>());
	CHECK(fs.get<1>().get<3>().size() == 1);
	CHECK(fs.get<1>().get<3>()[0] == 42);
	CHECK(fs.get<2>() == fd.get<2>());
	CHECK(fs.get<3>()[0] == fd.get<3>()[0]);
	CHECK(fs.get<3>()[1] == fd.get<3>()[1]);
}

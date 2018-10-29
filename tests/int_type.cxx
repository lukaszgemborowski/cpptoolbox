#include <toolbox/cpp/int_type.hpp>

// static checks

template<std::size_t N, typename T>
using check = std::is_same<typename toolbox::cpp::int_type<N>::signed_type, T>;

static_assert(check<1, std::int8_t>::value, "1 bit = char");
static_assert(check<8, std::int8_t>::value, "8 bit = char");

static_assert(check<9, std::int16_t>::value, "9 bit = int16");
static_assert(check<16, std::int16_t>::value, "16 bit = int16");

static_assert(check<17, std::int32_t>::value, "17 bit = int32");
static_assert(check<32, std::int32_t>::value, "32 bit = int32");

static_assert(check<33, std::int64_t>::value, "33 bit = int32");
static_assert(check<64, std::int64_t>::value, "64 bit = int32");

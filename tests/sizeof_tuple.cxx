#include <toolbox/cpp/sizeof_tuple.hpp>

using namespace toolbox::cpp;

static_assert(sizeof_tuple<std::tuple<char, int>>::value == 5, "tuple size should be 5");
static_assert(sizeof_tuple<std::tuple<char, short, int>>::value == 7, "tuple size should be 7");

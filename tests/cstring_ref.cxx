#include <toolbox/testing/test.hpp>
#include <toolbox/cpp/cstring_ref.hpp>

using namespace toolbox;

TEST_CASE(cstring_ref_construction)
{
    cstring_ref x{"abc123"};

// this shall produce compile time error
//    char non_const_arr[] = "abc123";
//    cstring_ref x1{non_const_arr};

// this also shall produce compile time error
//    cstring_ref x1{nullptr};

    CHECK(x.length() == 6);
    CHECK(x.length() == x.size());
}

TEST_CASE(cstring_ref_comparison)
{
    cstring_ref a1{"abc"};
    cstring_ref a2{"abc"};
    cstring_ref b{"def"};

    CHECK(a1 == a2);
    CHECK(a1 <= a2);
    CHECK(a1 >= a2);

    CHECK(a1 != b);
    CHECK(a1 < b);
    CHECK(b > a2);
}

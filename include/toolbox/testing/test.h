#ifndef _TOOLBOX_TESTING_H_
#define _TOOLBOX_TESTING_H_

#include <toolbox/config.h>
#include <stddef.h>
#if defined(TOOLBOX_HAS_STDLIB) && !defined(TOOLBOX_TEST_DISABLE_STDIO) && defined(TOOLBOX_TEST_MAIN)
#include <iostream>
#endif

namespace toolbox
{
namespace detail
{

struct test_results
{
    test_results() : current_check(0), success(true) {}
    int current_check;
    bool success;
};

#if defined(TOOLBOX_HAS_STDLIB) && !defined(TOOLBOX_TEST_DISABLE_STDIO) && defined(TOOLBOX_TEST_MAIN)

struct iostream_handler
{
    void start(const char *tc_name) const {
        std::cout << tc_name << "... ";
    }

    void stop(const test_results &res) const {
        if (res.success)
            std::cout << "PASSED" << std::endl;
        else
            std::cout << "FAILED" << std::endl;
    }
};
#else

struct null_handler
{
    void start(const char *) const {}
    void stop(const test_results &) const {}
};

#endif

struct base_test_case;

} // namespace detail
} // namespace toolbox

extern toolbox::detail::base_test_case *root_test;

namespace toolbox
{
namespace detail
{

struct base_test_case {
    test_results results;
    base_test_case *next;
    base_test_case() : next(NULL) {
        next = root_test;
        root_test = this;
    }

    virtual void run() {}
    virtual const char* name() const { return ""; }
};

} // namespace detail
} // namespace toolbox

#define TEST_CASE(tc_name) \
    struct tc_name##_test_case : public toolbox::detail::base_test_case { \
        void run(); \
        const char* name() const { return #tc_name; } \
    }; \
    tc_name##_test_case tc_name##_test_case_instance; \
    void tc_name##_test_case::run()


#define CHECK(expr) \
    results.current_check ++; \
    if (!(expr)) { \
        results.success = false; \
        return; }

#if defined(TOOLBOX_TEST_MAIN)
#define TEST_INIT \
    toolbox::detail::base_test_case *root_test = NULL;

namespace toolbox
{

#if defined(TOOLBOX_HAS_STDLIB) && !defined(TOOLBOX_TEST_DISABLE_STDIO)
template<typename ResultHandler = toolbox::detail::iostream_handler>
#else
template<typename ResultHandler = toolbox::detail::null_handler>
#endif
int test_run(ResultHandler handler = ResultHandler())
{
    toolbox::detail::base_test_case *current = root_test;
    int failedCount = 0;

    while (current) {
        handler.start(current->name());
        current->run();
        handler.stop(current->results);

        if (current->results.success == false)
            failedCount ++;

        current = current->next;
    }

    return failedCount;
}

} // namespace toolbox
#endif

#endif // _TOOLBOX_TESTING_H_

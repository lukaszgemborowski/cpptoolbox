#ifndef _TOOLBOX_ARGV_OPTIONS_HPP_
#define _TOOLBOX_ARGV_OPTIONS_HPP_

#include <functional>
#include <string>
#include <vector>
#include "toolbox/config.h"
#if TOOLBOX_CXX_STANDARD >= 17
# include <string_view>
#endif

namespace toolbox
{
namespace argv
{

namespace detail
{

template<typename T>
struct value_container
{
    using type_t = T;
    using element_type_t = T;
    using is_void = std::false_type;

    T value_;
    const auto& set(const char *p)
    {
        std::stringstream ss(p);
        ss >> value_;
        return value_;
    }

    const T& get() const
    {
        return value_;
    }
};

template<typename T>
struct value_container<std::vector<T>>
{
    using type_t = std::vector<T>;
    using element_type_t = T;
    using is_void = std::false_type;

    std::vector<T> value_;
    const auto& set(const char *p)
    {
        std::stringstream ss(p);
        T temp;
        ss >> temp;
        value_.push_back(temp);
        return value_.back();
    }

    const auto& get() const
    {
        return value_;
    }
};

template<>
struct value_container<void>
{
    using type_t = bool;
    using element_type_t = bool;
    using is_void = std::true_type;

    bool value_ = false;
    const auto& set(const char *)
    {
        value_ = true;
        return value_;
    }

    const bool& get() const
    {
        return value_;
    }
};

template<typename T> struct function_type
{
    using have_argument_t = std::true_type;
    using type_t = std::function<void (T &)>;
};

template<> struct function_type<void>
{
    using have_argument_t = std::false_type;
    using type_t = std::function<void ()>;
};

} // namespace detail

struct base_option
{
public:
#if TOOLBOX_CXX_STANDARD >= 17
    base_option(char name, std::string_view long_name)
#else
    base_option(char name, const std::string& long_name)
#endif
        : short_name_ {name}
        , long_name_ {long_name}
        , description_ {}
    {
    }

    char get_short() const
    {
        return short_name_;
    }

    const std::string &get_long() const
    {
        return long_name_;
    }

    const std::string &get_description() const
    {
        return description_;
    }

    const std::string &get_argument_name() const
    {
        return argument_name_;
    }

    auto found() const
    {
        return found_;
    }

protected:
    const char          short_name_;
    const std::string   long_name_;
    std::string         description_ {};
    std::string         argument_name_ {};
    unsigned            found_  {0u};
};

template<typename T = void>
struct option : public base_option
{
#if TOOLBOX_CXX_STANDARD >= 17
    option(char name, std::string_view long_name = std::string_view{}) :
#else
    option(char name, const std::string &long_name = std::string{}) :
#endif
        base_option (name, long_name)
    {
    }

    bool has_argument() const
    {
        return detail::value_container<T>::is_void::value == false;
    }

    typename detail::value_container<T>::type_t
    value() const
    {
        return value_.get();
    }

    using callback_type = std::function<void (const typename detail::value_container<T>::element_type_t &)>;

    option<T>& action(const callback_type &func)
    {
        func_ = func;
        return *this;
    }

    option& description(const std::string &desc, const std::string &arg_name = std::string())
    {
        description_ = desc;
        argument_name_ = arg_name;
        return *this;
    }

    option<T>& store(typename detail::value_container<T>::type_t &dest)
    {
        transfer_to_storage_ = [&dest](const typename detail::value_container<T>::type_t &src) {
            dest = src;
        };

        return *this;
    }

    void set_found(const char *arg)
    {
        found_ ++;
        const auto &value = value_.set(arg);
        if (func_)
            func_(value);
    }

    void transfer_to_storage() const
    {
        if (transfer_to_storage_)
            transfer_to_storage_(value_.get());
    }

private:
    detail::value_container<T> value_;
    callback_type func_;
    std::function<void (const typename detail::value_container<T>::type_t &)> transfer_to_storage_;
};

} // namespace argv
} // namespace toolbox

#endif

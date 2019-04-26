#ifndef TOOLBOX_CPP_CSTRING_REF_HPP
#define TOOLBOX_CPP_CSTRING_REF_HPP

#include <cstring>
#include <string>
#include "toolbox/cpp/span.hpp"
#include "toolbox/std/type_traits.hpp"

namespace toolbox
{

template<class CharT>
struct basic_cstring_ref : public span<const CharT>
{
    template<class T>
    explicit basic_cstring_ref(T *cstr) noexcept
        : span<const CharT>(cstr, cstr + std::strlen(cstr))
    {
        static_assert(std::is_const<T>::value == true, "c-string must be const");
    }

    basic_cstring_ref(nullptr_t) = delete;

    auto length() const noexcept {
        return span<const CharT>::size();
    }

    auto c_str() const noexcept {
        return span<const CharT>::begin();
    }

    auto operator < (const basic_cstring_ref &rhs) const noexcept {
        return std::strcmp(c_str(), rhs.c_str()) < 0;
    }

    auto operator < (CharT const *rhs) const noexcept {
        return std::strcmp(c_str(), rhs) < 0;
    }

    auto operator <= (const basic_cstring_ref &rhs) const noexcept {
        auto r = std::strcmp(c_str(), rhs.c_str());
        return r == 0 || r < 0;
    }

    auto operator <= (CharT const *rhs) const noexcept {
        auto r = std::strcmp(c_str(), rhs);
        return r == 0 || r < 0;
    }

    auto operator > (const basic_cstring_ref &rhs) const noexcept {
        return std::strcmp(c_str(), rhs.c_str()) > 0;
    }

    auto operator > (CharT const *rhs) const noexcept {
        return std::strcmp(c_str(), rhs) > 0;
    }

    auto operator >= (const basic_cstring_ref &rhs) const noexcept {
        auto r = std::strcmp(c_str(), rhs.c_str());
        return r == 0 || r > 0;
    }

    auto operator >= (CharT const *rhs) const noexcept {
        auto r = std::strcmp(c_str(), rhs);
        return r == 0 || r > 0;
    }

    auto operator == (const basic_cstring_ref &rhs) const noexcept {
        return std::strcmp(c_str(), rhs.c_str()) == 0;
    }

    auto operator == (CharT const *rhs) const noexcept {
        return std::strcmp(c_str(), rhs) == 0;
    }

    auto operator != (const basic_cstring_ref &rhs) const noexcept {
        return !(*this == rhs);
    }

    auto operator != (CharT const *rhs) const noexcept {
        return !(*this == rhs);
    }

    operator std::basic_string<CharT> () const {
        return std::basic_string<CharT>(c_str());
    }
};

using cstring_ref = basic_cstring_ref<char>;

} // namespace toolbox

#endif // TOOLBOX_CPP_CSTRING_REF_HPP

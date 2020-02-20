#ifndef TOOLBOX_STRING_LINE_ITERATOR_HPP
#define TOOLBOX_STRING_LINE_ITERATOR_HPP

#include <string>
#include <string_view>

namespace toolbox::string
{

template<
    class CharT,
    class Traits,
    class Allocator>
class line_iterator
{
private:
    using string_t = std::basic_string<CharT, Traits, Allocator>;
    using string_view_t = std::basic_string_view<CharT, Traits>;

public:
    line_iterator(const string_t &str)
        : str_ {&str}
        , pos_ {0}
        , len_ {str.find(CharT{'\n'})}
    {
        if (str_->size() == 0) {
            str_ = nullptr;
            len_ = 0;
        }
    }

    line_iterator()
        : str_ {nullptr}
        , pos_ {0}
        , len_ {0}
    {}

    string_view_t operator*() const
    {
        return {str_->c_str() + pos_, len_};
    }

    line_iterator& operator++()
    {
        pos_ += len_ + 1;
        auto p = str_->find(CharT{'\n'}, pos_);

        if (p != string_t::npos) {
            len_ = p - pos_;
        } else {
            if (pos_ >= str_->size()) {
                str_ = nullptr;
                pos_ = 0;
                len_ = 0;
            } else {
                len_ = str_->size() - pos_;
            }
        }

        return *this;
    }

    bool operator==(const line_iterator &rhs) const
    {
        return
            str_ == rhs.str_ &&
            pos_ == rhs.pos_ &&
            len_ == rhs.len_;
    }

    bool operator!=(const line_iterator &rhs) const
    {
        return !(*this == rhs);
    }

    auto pos() const { return pos_; }

private:
    const string_t *str_;
    typename string_t::size_type pos_;
    typename string_t::size_type len_;
};

line_iterator() -> line_iterator<char, std::char_traits<char>, std::allocator<char>>;

} // namespace toolbox::string

#endif // TOOLBOX_STRING_LINE_ITERATOR_HPP
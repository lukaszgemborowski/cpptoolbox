#ifndef TOOLBOX_STRING_LINE_ITERATOR_HPP
#define TOOLBOX_STRING_LINE_ITERATOR_HPP

#include <string>
#include <string_view>

namespace toolbox::string
{

template<class String>
class basic_line_reference;

template<class String>
class line_iterator
{
private:
    using string_t = String;
    using CharT = typename String::value_type;
    using string_view_t = std::basic_string_view<
        typename String::value_type,
        typename String::traits_type>;

public:
    line_iterator(string_t &str)
        : str_ {&str}
        , pos_ {0}
        , len_ {str.find(CharT{'\n'})}
        , lineNumber_ {0}
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
        , lineNumber_ {0}
    {}

    string_view_t get_string_view() const
    {
        return {str_->c_str() + pos_, len_};
    }

    auto operator*()
    {
        return basic_line_reference<String>{*this};
    }

    line_iterator& operator++()
    {
        lineNumber_ ++;
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

    auto lineNumber() const
    {
        return lineNumber_;
    }

    void recalculate()
    {
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
    }

    auto position() const { return pos_; }
    auto length() const { return len_; }
    auto& string() { return *str_; }

private:
    string_t *str_;
    typename string_t::size_type pos_;
    typename string_t::size_type len_;
    std::size_t lineNumber_;
};

line_iterator() -> line_iterator<std::string>;

template<class String>
class basic_line_reference
{
    using iterator_t = line_iterator<String>;
    using string_t = String;
    using string_view_t = std::basic_string_view<
        typename String::value_type,
        typename String::traits_type>;

public:
    using size_type = typename string_t::size_type;

    basic_line_reference(iterator_t &iter)
        : iter_ {iter}
    {
    }

    auto operator*() const
    {
        return iter_.get_string_view();
    }

    auto lineNumber() const { return iter_.lineNumber(); }
    auto length() const { return iter_.length(); }

    void recalculate()
    {
        iter_.recalculate();
    }

    // std::basic_string-like interface
    basic_line_reference& replace(size_type pos, size_type count, const std::string &str)
    {
        iter_.string().replace(pos + iter_.position(), count, str);
        return *this;
    }

    basic_line_reference& replace(size_type pos, size_type count, string_view_t str)
    {
        iter_.string().replace(pos + iter_.position(), count, str);
        return *this;
    }

    auto substr(size_type pos, size_type count)
    {
        return iter_.get_string_view().substr(pos, count);
    }

private:
    iterator_t& iter_;
};

using line_reference = basic_line_reference<std::string>;

} // namespace toolbox::string

#endif // TOOLBOX_STRING_LINE_ITERATOR_HPP

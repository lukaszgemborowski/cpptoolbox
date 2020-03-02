#ifndef TOOLBOX_TEXT_FILE_HPP
#define TOOLBOX_TEXT_FILE_HPP

#include <toolbox/cpp/filesystem.hpp>
#include <fstream>
#include <streambuf>

namespace toolbox
{

class text_file
{
public:
    text_file(const fs::path &path)
        : path_ {path}
        , content_ {load_as_string(path)}
    {}

    static std::string load_as_string(std::ifstream &file)
    {
        std::string result;

        file.seekg(0, std::ios::end);
        result.reserve(file.tellg());
        file.seekg(0, std::ios::beg);

        result.assign(std::istreambuf_iterator<char>(file),
                    std::istreambuf_iterator<char>());
        return result;
    }

    static std::string load_as_string(toolbox::fs::path path)
    {
        std::ifstream ifs{path};
        return load_as_string(ifs);
    }

    static void save_from_string(std::ofstream &file, const std::string &str)
    {
        file.write(str.data(), str.size());
    }

    static void save_from_string(fs::path path, const std::string &str)
    {
        std::ofstream ofs{path};
        save_from_string(ofs, str);
    }

    void save()
    {
        save_from_string(path_, content_);
    }

    std::string& content()
    {
        return content_;
    }

    const std::string& content() const
    {
        return content_;
    }

    template<template <typename> typename Iterator>
    auto iterator()
    {
        return IteratorWrapper<Iterator<std::string>>{content_};
    }

private:
    template<class Iterator>
    class IteratorWrapper
    {
    public:
        IteratorWrapper(std::string &content)
            : content_ {content}
        {}

        auto begin()
        {
            return Iterator{content_};
        }

        auto end()
        {
            return Iterator{};
        }

    private:
        std::string &content_;
    };


private:
    fs::path    path_;
    std::string content_;
};



} // namespace toolbox

#endif // TOOLBOX_TEXT_FILE_HPP

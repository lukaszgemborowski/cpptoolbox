#ifndef TOOLBOX_TEXT_FILE_HPP
#define TOOLBOX_TEXT_FILE_HPP

#include <toolbox/cpp/filesystem.hpp>
#include <fstream>
#include <streambuf>

namespace toolbox::text_file
{

std::string load(std::ifstream &file)
{
    std::string result;

    file.seekg(0, std::ios::end);
    result.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    result.assign(std::istreambuf_iterator<char>(file),
                  std::istreambuf_iterator<char>());
    return result;
}

std::string load(toolbox::fs::path path)
{
    std::ifstream ifs{path};
    return load(ifs);
}

void save(std::ofstream &file, const std::string &str)
{
    file.write(str.data(), str.size());
}

void save(fs::path path, const std::string &str)
{
    std::ofstream ofs{path};
    save(ofs, str);
}

} // namespace toolbox::text_file

#endif // TOOLBOX_TEXT_FILE_HPP
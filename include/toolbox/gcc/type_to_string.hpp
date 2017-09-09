#ifndef _TOOLBOX_GCC_TYPE_TO_STRING_HPP_
#define _TOOLBOX_GCC_TYPE_TO_STRING_HPP_

#include <typeinfo>
#include <cxxabi.h>

namespace toolbox
{
namespace gcc
{

template<typename T>
std::string type_to_string()
{
	int status = 0;
	return abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
}

template<typename T>
std::string type_to_string(const T&)
{
	return type_to_string<T>();
}

} // namespace gcc
} // namespace toolbox

#endif

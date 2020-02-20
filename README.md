# cpptoolbox

[![Build Status](https://travis-ci.org/lukaszgemborowski/cpptoolbox.svg?branch=master)](https://travis-ci.org/lukaszgemborowski/cpptoolbox)

My C++ utilities library with some common C++ constructs I'm using in several different kind of projects. You will find here modern c++ high level utilities abstracting some common Linux operations and also things I've used in my embedded project, even some stuff from 8-bit AVR microcontrollers. Therefore many classes and utilities does not even require compiler to support standard C++ library. The project itself tries to emulate some standard things such as type_traits if libstdc++/libc++ is not available. This requires generation (with cmake) of config.h before using the library (look at cmake/config.h.in).

## Thanks
My library is using Eric Niebler meta lib: https://github.com/ericniebler/meta header files are placed in include/meta directory.

## Help
Currently the library consists of several "modules":

## 1. cpp [[doc]](doc/toolbox/cpp/cpp.md)
Some common C++ helpers related only to language and standard library

### 1.1. call_seq
wraps several functors into one callable which is sequence of functors. To do that your functors signatures needs to match. Eg.:

```cpp
auto seq = toolbox::cpp::make_call_seq(
		[](auto &value) {
			value += 2;
		},
		[](auto &value) {
			value *= 2;
		}
);

auto value = 2;
seq(value);
```
first value will be increased by 2 and than multiplied by 2.

### 1.2. for_each_param
convinient meta-function for iterating over all variadic arguments passed to the function.

```cpp
template<typename... Args>
auto printer(std::ostream &os, Args... args)
{
	toolbox::cpp::for_each_param(
		[&os](auto arg) {
			os << " -> " << arg << std::endl;
		},
		args...
	);
}
```

### 1.3. make_ref_tuple
makes a tuple but if lvalue is provided it takes reference to it instead of a copy.

```cpp
auto a = 1;
auto t = toolbox::cpp::make_ref_tuple(a, 2);
```

will produce `std::tuple<int &, int>` with a reference to `a` as first element.

### 1.4. tuple_at(tuple, index, func)
applies functor `func` on `index`-th element of tuple. What's important indexing is done in runtime by linear evaluation of elements, so access is O(N) and not O(1) as might be expected.

### 1.5. tuple_for_each(tuple, func)
applies `func` on every element of `tuple` in order. If you have non-compatible types in the tuple (ie. they cannot be automatically converted to your functor's argument type) you need to provide functor with templated `operator()` or lambda with `auto` argument.

## 2. curl
libcurl C++ wrapper inspired by libcurlpp and providing only small subset of functionalities

### 2.1. global
just a RAII wrapper for curl initialzation and deinitialization, basically it's calling `curl_global_init` in ctor and `curl_global_cleanup` in dtor.

### 2.2. easy
C++ implementation of curl's easy interface. To perform curl request you need to create `easy` object
with parameters. Please use `make_easy` helper function to do that:

eg.:
```cpp
auto request = toolbox::curl::make_easy(
	toolbox::curl::options::verbose{1},
	toolbox::curl::options::url{"http://google.com"},
	toolbox::curl::options::httpheader({"Abc: test", "Def: test2"})
);

request.perform();
```

## 3. gcc
gcc related stuff

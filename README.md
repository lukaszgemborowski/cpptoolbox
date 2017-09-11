# cpptoolbox
My C++ utilities library with some common C++ constructs I'm using. The goal is to keep it small and header only. Currently the library consists of several "modules":

## 1. argv [[doc]](doc/toolbox/argv/argv.md)
Command line argument parsing (getopt wrapper)

### 1.1. parser [[doc]](doc/toolbox/argv/parser.md)
idea behind this wrapper is a little bit different than in most of the other parsers. You do not fetch parsed options by name (such as `parser.get('help')`) but instead you define option objects (`option` class) which acts both as a definition and a proxy object for argument value. First you define option objects and than based on that you create the parser

```cpp
namespace o = toolbox::argv::options;
auto help = o::option<void>(o::short_name('h'), o::long_name("help")).description("this help message");
auto verbose = o::option<void>(o::short_name('v'), o::long_name("verbose")).description("be verbose");
auto value = o::option<int>(o::short_name('n'));

auto parser = toolbox::argv::make_parser(help, verbose, value);
parser.parse(argc, argv);
```

now you can use previously created objects to access parsed arguments, eg.:

```cpp
std::cout << help.value() << ", " << verbose.value() << ", " << value.value() << std::endl;
```

keep in mind that `make_parser` will get references to your option objects so you need to keep them alive.

`option<T>`'s T template argument defines how the argument is parsed:
1. `void` - `value()` returns `bool` indicating if option was found (eg. --help)
2. any type which can be converted by `std::stringstream`, in such case `value()` returns `T`
3. `std::vector<some type>` - as 3. but holds vector of values (eg. -I path -I other/path), `value()` will return `std::vector<type>`

## 2. cpp [[doc]](doc/toolbox/cpp/cpp.md)
Some common C++ helpers related only to language and standard library

### 2.1. call_seq
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

### 2.2. for_each_param
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

### 2.3. make_ref_tuple
makes a tuple but if lvalue is provided it takes reference to it instead of a copy.

```cpp
auto a = 1;
auto t = toolbox::cpp::make_ref_tuple(a, 2);
```

will produce `std::tuple<int &, int>` with a reference to `a` as first element.

### 2.4. tuple_at(tuple, index, func)
applies functor `func` on `index`-th element of tuple. What's important indexing is done in runtime by linear evaluation of elements, so access is O(N) and not O(1) as might be expected.

### 2.5. tuple_for_each(tuple, func)
applies `func` on every element of `tuple` in order. If you have non-compatible types in the tuple (ie. they cannot be automatically converted to your functor's argument type) you need to provide functor with templated `operator()` or lambda with `auto` argument.

## 3. curl
libcurl C++ wrapper inspired by libcurlpp and providing only small subset of functionalities

### 3.1. global
just a RAII wrapper for curl initialzation and deinitialization, basically it's calling `curl_global_init` in ctor and `curl_global_cleanup` in dtor.

### 3.2. easy
C++ implementation of curl's easy interface. To perform curl request you need three things:

1. create `tuple` of toolbox::curl::options::\* objects to configure curl request
2. pass the options to easy request by calling `easy::set()`
3. call `perform()` method of easy interface to execute the request

eg.:
```cpp
auto global = toolbox::curl::global(toolbox::curl::flags::Default);
auto request = toolbox::curl::easy{};
auto options = std::make_tuple(
    toolbox::curl::options::verbose{1},
    toolbox::curl::options::url{"http://google.com"},
    toolbox::curl::options::httpheader({"Abc: test", "Def: test2"})
);

request.use(options);
request.perform();
```

## 4. gcc
gcc related stuff

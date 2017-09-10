# cpptoolbox
My C++ utilities library with some common C++ constructs I'm using. The goal is to keep it small and header only. Currently the library consists of several "modules":

## 1. argv
Command line argument parsing (getopt wrapper)

### 1.1. parser
idea behind this wrapper is a little bit different in most of the other parsers. You do not fetch parsed options by name (such as `parser.get('help')`) but instead you define option objects (option class) which acts both as a definition and a proxy object for argument value. First you define some object and than based on that you create the parser

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

`option<T>` T template argument defines how the argument is parsed:
1. `void` - `value()` returns `bool` indicating if option was found (eg. --help)
2. `toolbox::argv::options::counter` - `value()` returns int. It counts how many times option was found in command line (eg. -v -vv -vvv etc)
3. `other` - any type which can be converted by `std::stringstream`, in such case `value()` returns `T`
4. `std::vector<some type>` - as 3. but holds vector of values (eg. -I path -I other/path), `value()` will return `std::vector<type>`

**WARNING:** as for now there is no validation implemented!

## 2. cpp
Some common C++ helpers related only to language and standard library

### 2.1. call_seq
wraps several functors into one callable which is sequence of functors. To do that your functor signatures needs to match. Eg.:

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

### 2.2. make_ref_tuple
makes a tuple but if lvalue is provided it takes reference to it instead of a copy.

```cpp
auto a = 1;
auto t = toolbox::cpp::make_ref_tuple(a, 2);
```

will produce `std::tuple<int &, int>` with a reference to `a` as first element.

## 3. curl
libcurl C++ wrapper inspired by libcurlpp providing only small subset of functionalities

## 4. gcc
gcc related stuff

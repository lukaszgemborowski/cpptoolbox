# cpp

general C++ and standard library helpers

## call_seq
```cpp
template<typename... Functors> auto make_call_seq(Functors... functors);
```
aggregate several functors into single one

example:
```cpp
auto seq = toolbox::cpp::make_call_seq(
		[](auto &value) {
			value += 3;
		},
		[](auto &value) {
			value *= 2;
		}
);

auto value = 2;
std::cout << seq(value);
```

will result in "10"

## for_each_param
```cpp
template<typename Func, typename... Params> auto for_each_param(Func &&func, Params&&... params);
```
call func for each of the `params...`

example:
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

## make_ref_tuple
```cpp
template<typename Head, typename... Tail> auto make_ref_tuple(Head &head, Tail&&... tail);
```
makes a tuple but if lvalue is provided it takes reference to it instead of a copy.
```cpp
auto a = 1;
auto t = toolbox::cpp::make_ref_tuple(a, 2);
```
will produce std::tuple<int &, int> with a reference to a as first element.

## tuple_at
```cpp
template<typename T, typename F> F tuple_at(T &&tuple, std::size_t index, F &&func);
```
applies functor `func` on `index`-th element of tuple. What's important indexing is done in runtime by linear evaluation of elements, so access is O(N) and not O(1) as might be expected. This function is based on tuple_for_each implementation

## tuple_for_each
```cpp
template<typename T, typename F> F tuple_for_each(T &&tuple, F &&func);
```
applies `func` on every element of `tuple` in order. If you have non-compatible types in the tuple (ie. they cannot be automatically converted to your functor's argument type) you need to provide functor with templated `operator()` or lambda with `auto` argument.

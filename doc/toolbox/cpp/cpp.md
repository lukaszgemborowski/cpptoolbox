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

## tuple_at
```cpp
template<typename T, typename F> F tuple_at(T &&tuple, std::size_t index, F &&func);
```

## tuple_for_each
```cpp
template<typename T, typename F> F tuple_for_each(T &&tuple, F &&func);
```

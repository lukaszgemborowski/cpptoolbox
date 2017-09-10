# options

options definition

```cpp
template<typename T = void> struct option;
```

there is only one template argument `T` which defines how option is parsed. There are three general
formats of `T`:

1. `void` - checks if option was present in command line (without additional parameter)
2. `type` - parameter of type `type`, uses `std::stringstream` to parse parameter
3. `std::vector<type>` - as 2. but stores multiple occurences of argument in the std::vector

## option::option
```cpp
option::option(const short_name &name, const long_name &lname = long_name{});
```

create option definition with provided short and long name, short name is one letter, long name
ay contain more than one letter. When parsed short name is prefixed by '-' and long name is prefixed
by '--'. To create option:

```cpp
auto help = option<void>(short_name('h'), long_name('help'));
```

## option::value
```cpp
auto option::value() const;
```

will return parsed parameter of given option. Return value in most cases is `T` but in case of `T == void`
it's `bool` indicating if option was found in command line

## option::found
```cpp
unsigned option::found() const;
```

returns how many times option was present in command line, you can use this function to check option presence.

## option::action
```cpp
option<T>& option::action(const callback_type &func);
```

allows to register callback called when option is found during command line parsing. Exact function signature
depends on `T`. There are three cases:

1. `T == void` - `std::function<void (const bool &)>`
2. `T` - `std::function<void (const T &)>`
3. `T == std::vector<U>` - `std::function<void (const U &)>`

## option::store
```cpp
option<T>& option::store(typename detail::value_container<T>::type_t &dest)
```

store parsed value in provided variable

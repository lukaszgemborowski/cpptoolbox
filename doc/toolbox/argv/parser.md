# toolbox::argv::parser
```cpp
template<typename Options> class parser;
```

main parser class

## parser::parser
```cpp
parser::parser(Options &&options);
```
parser constructor, it `std::move`s options and build everything which is needed for getopt to parse
command line arguments

## parser::parse
```cpp
void parser::parse(int argc, char ** argv);
```

runs `getopt_long()` to actually parse command line arguments, also this is the place from where
all user defined callbacks will be called (defined by `option::action`)

## parser::non_options
```cpp
const std::vector<std::string> &parser::non_options() const;
```

get reference to `std::vector` of non-option values, a non-option value is everything which was not
recognized during parsing

## parser::print_options
```cpp
void parser::print_options(std::ostream &os) const;
```

dumps all available options to `os` stream

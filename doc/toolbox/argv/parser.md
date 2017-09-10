# toolbox::argv::parser
```cpp
template<typename Options> class parser;
```

command line arguments pareser class, it uses getopt as actual parser and it's basically c++ wrapper
around it with some additional bonuses. The idea behind this class is a little bit different than in
other parsers. It does not  use argument names directly as for example in `boost.program_options`
where to access argument you need to type its name as string literal, eg. `vm["some-option"].as<std::string>()`
but instead you have to create option object, specify argument name, type, etc. and than use this object
to actually access parsed value.

```cpp
auto number = o::option<int>(o::short_name('n'));
auto parser = toolbox::argv::make_parser(number);
parser.parse(argc, argv);
```
now you can use `number` object as handle to parsed value, there's no need to write 'n' anywhere else in
the program. Thanks to that you can't mistake option name or type in the future. `number` will always
reference to 'n' and will always return `int` whenever requested.

## parser::parser
```cpp
parser::parser(Options &&options);
```
parser constructor, it `std::move`s options and build everything which is needed for getopt to parse
command line arguments. There is utility function [make_parser](make_parser.md) which helps to construct
actual parser object and deduce template argument list. I recommend you to use this function.

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

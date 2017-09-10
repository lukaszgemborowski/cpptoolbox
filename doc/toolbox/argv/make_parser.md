# toolbox::argv::make_parser
```cpp
template<typename... Options> auto make_parser(Options&&... options);
```
utility function for creating parser object based on provided list of options. It helps to
deduce actual parser template argument list.

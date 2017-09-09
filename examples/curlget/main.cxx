#include <toolbox/curl/global.hpp>
#include <toolbox/curl/easy.hpp>
#include <toolbox/argv/parser.hpp>
#include <cstdio>
#include <iostream>

int main(int argc, char **argv)
{
	// default is stdout
	FILE* out = stdout;

	// handy shortcut for options parser
	namespace o = toolbox::argv::options;

	// user may pass -v option meaning "verbose"
	auto verbose = o::option<void>(o::short_name('v'));

	// more complex option, if user pass -o some/path.ext callback (lambda)
	// will be called with user provided path, this lambda will open file
	// for writing
	auto output = o::option<std::string>(o::short_name('o'))
					.action(
						[&out](const std::string &path) {
							out = fopen(path.c_str(), "w");
					});

	// create parser which understands verbose and output options
	auto parser = toolbox::argv::make_parser(verbose, output);

	// parse
	parser.parse(argc, argv);

	// there should be one free argument - url to get
	if (parser.non_options().size() != 1) {
		std::cerr << "you need to provide url" << std::endl;
		return 1;
	}

	// create global curl instance, this is for curl initialization and cleanup
	auto curl = toolbox::curl::global(toolbox::curl::flags::Default);

	// prepare options for curl
	namespace opt = toolbox::curl::options;
	auto options = toolbox::curl::make_options(
		opt::verbose(verbose.value()),
		opt::url(parser.non_options()[0]),

		// default behavior of libcurl is to write response
		// to writedata FILE pointer, may be stdout
		opt::writedata(out)
	);

	auto easy = toolbox::curl::easy{};
	easy.use(options);
	easy.perform();

	fclose(out);
	return 0;
}

#include <toolbox/curl/global.hpp>
#include <toolbox/curl/easy.hpp>
#include <toolbox/argv.hpp>
#include <cstdio>
#include <iostream>

int main(int argc, char **argv)
{
	// default is stdout
	FILE* out = stdout;

	// handy shortcut for options parser
	namespace arg = toolbox::argv;

	auto help = arg::option<void>('h', "help").description("this help message");

	// user may pass -v option meaning "verbose"
	auto verbose = arg::option<void>('v', "verbose").description("be verbose");

	// more complex option, if user pass -o some/path.ext callback (lambda)
	// will be called with user provided path, this lambda will open file
	// for writing
	auto output = arg::option<std::string>('o', "output")
					.description("save downloaded file to FILE", "FILE")
					.action(
						[&out](const std::string &path) {
							out = fopen(path.c_str(), "w");
					});

	// create parser which understands verbose and output options
	auto parser = toolbox::argv::make_parser(help, verbose, output);

	// parse
	parser.parse(argc, argv);

	if (help.value()) {
		parser.print_options(std::cout);
		fclose(out);
		return 0;
	}

	// there should be one free argument - url to get
	if (parser.non_options().size() != 1) {
		std::cerr << "you need to provide url" << std::endl;
		return 1;
	}

	// create global curl instance, this is for curl initialization and cleanup
	auto curl = toolbox::curl::global(toolbox::curl::flags::Default);

	// prepare easy object with preconfigured options
	namespace opt = toolbox::curl::options;
	auto easy = toolbox::curl::make_easy(
		opt::verbose(verbose.value()),
		opt::url(parser.non_options()[0]),

		// default behavior of libcurl is to write response
		// to writedata FILE pointer, may be stdout
		opt::writedata(out)
	);

	easy.perform();

	fclose(out);
	return 0;
}

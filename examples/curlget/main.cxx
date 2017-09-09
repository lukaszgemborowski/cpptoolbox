#include <toolbox/curl/global.hpp>
#include <toolbox/curl/easy.hpp>
#include <toolbox/argv/parser.hpp>
#include <cstdio>
#include <iostream>

int main(int argc, char **argv)
{
	FILE* out = stdout;

	namespace o = toolbox::argv::options;
	auto verbose = o::option<void>(o::short_name('v'));
	auto output = o::option<std::string>(o::short_name('o'))
					.action(
						[&out](const std::string &path) {
							out = fopen(path.c_str(), "w");
					});

	auto parser = toolbox::argv::make_parser(verbose, output);
	parser.parse(argc, argv);

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

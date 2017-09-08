#include <toolbox/curl/global.hpp>
#include <toolbox/curl/easy.hpp>
#include <cstdio>

int main(int argc, char **argv)
{
	FILE* out = nullptr;

	if (argc == 3) {
		out = fopen(argv[2], "w");
	} else if (argc == 2) {
		out = stdout;
	} else {
		return 1;
	}

	// create global curl instance, this is for curl initialization and cleanup
	auto curl = toolbox::curl::global(toolbox::curl::flags::Default);

	// prepare options for curl
	namespace opt = toolbox::curl::options;
	auto options = toolbox::curl::make_options(
		opt::verbose(1),
		opt::url(argv[1]),

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

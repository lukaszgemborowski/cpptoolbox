#define TOOLBOX_TEST_MAIN
#include <toolbox/testing/test.h>

TEST_INIT;

int main(int, char**)
{
	return toolbox::test_run();
}
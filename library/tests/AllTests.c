#include "unity_fixture.h"

static void RunAllTests(void)
{
	RUN_TEST_GROUP(sdcard_interface);
	RUN_TEST_GROUP(file_interface);
}

int main(int argc, char * argv[])
{
return UnityMain(argc, argv, RunAllTests);
}

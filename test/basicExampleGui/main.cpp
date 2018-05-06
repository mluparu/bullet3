#include <string>
#include <gtest/gtest.h>
using namespace std;

int runSimulation(int argc, char* argv[]);

TEST(BasicExampleGui, TimeCount) {
	auto duration = runSimulation(0, nullptr);
	ASSERT_GT(duration, 4);
}

int main(int argc, char** argv) {
#if _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//void *testWhetherMemoryLeakDetectionWorks = malloc(1);
#endif
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

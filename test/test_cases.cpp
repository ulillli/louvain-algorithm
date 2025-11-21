#include <gtest.h>
#include <utility>
#include "louvain.h"

const int count = 8;

class LouvainFileTests : public testing::TestWithParam<std::string> {
protected:
	std::string filename;

	void SetUp() override {
		filename = GetParam();
	}
};
std::vector<std::string> GetTestFiles() {
	std::vector<std::string> files;
	for (int i = 1; i <= count; i++) {
		files.push_back("../../small_data/graph" + std::to_string(i) + ".txt");
	}
	return files;
}
TEST_P(LouvainFileTests, LoadsGraphSuccessfully) {
	graph G(filename.data());
	EXPECT_GT(G.getVertexCount(), 0) << "Graph should have vertices. File: " << filename;
}
TEST_P(LouvainFileTests, LouvainMethodIsWorking) {
	graph G(filename.data());
	louvain_new L(G);
	EXPECT_GT(L.getCommunitiesCount(), 0) << "Count of communities = 0. File: " << filename;
}
TEST_P(LouvainFileTests, ModularityValidation) {
	graph G(filename.data());
	louvain_new L(G);
	std::vector<int> partition = L.getPartition();
	ASSERT_EQ(-1<=L.getModularity(G,partition)<=1, 1) << "Modularity isn't in range [-1;1]. File: " << filename;
}
INSTANTIATE_TEST_CASE_P( GraphFiles,
	LouvainFileTests,
	::testing::ValuesIn(GetTestFiles()),
	[](const ::testing::TestParamInfo<std::string>& info) {
		// Генерируем читаемые имена тестов
		std::string name = "Graph" + std::to_string(info.index);
		return name;
	}
);

TEST(louvainAlgorithm, FictitiousTest)
{
	EXPECT_EQ(0, 0);
}

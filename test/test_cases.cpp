#include <gtest.h>
#include <utility>
#include "louvain.h"

const int count = 11;

class LouvainFileTests : public testing::TestWithParam<std::string> {
protected:
	std::string filename;
	std::vector<float> edges;
	std::vector<float> start_modularity, finish_modularity;
	std::vector<int> communities_count;
	void SetUp() override {
		filename = GetParam();
		std::ifstream file("../../data/answersFromPython/answers.txt", std::ios::in);
		edges = std::vector<float>(count);
		start_modularity = std::vector<float>(count);
		finish_modularity = std::vector<float>(count);
		communities_count = std::vector<int>(count);
		for (int i = 0; i < count; i++) {
			float m, first_modulairty, second_modularity;
			int n,count;
			file >> n >> m >> first_modulairty >> second_modularity >> count;
			edges[i] = m;
			start_modularity[i] = first_modulairty;
			finish_modularity[i] = second_modularity;
			communities_count[i] = count;
		}
	}
};
std::vector<std::string> GetTestFiles() {
	std::vector<std::string> files;
	for (int i = 1; i <= count; i++) {
		files.push_back("../../small_data/graph" + std::to_string(i) + ".txt");
	}
	return files;
}
TEST_P(LouvainFileTests, CheckingAnswers) { 
	graph G(filename.data());
	louvain L(G);
	std::vector<int> partition = L.getPartition();
	float first_modularity = L.getFirstModularity();
	float second_modularity = L.getModularity(G, partition);
	float M = G.getEdgeCount();
	int i=22;
	std::string s = "";
	while (filename[i] != '.') {
		s += filename[i];
		i++;
	}
	int num = std::stoi(s)-1;
	float eps = 0.1f;
	EXPECT_EQ(true, edges[num] == M && abs(start_modularity[num] - first_modularity) <= eps && abs(finish_modularity[num] - second_modularity) <= eps && abs(communities_count[num] - L.getCommunitiesCount()) <= 4);
}
TEST_P(LouvainFileTests, LoadsGraphSuccessfully) {
	graph G(filename.data());
	EXPECT_GT(G.getVertexCount(), 0) << "Graph should have vertices. File: " << filename;
}
TEST_P(LouvainFileTests, LouvainMethodIsWorking) {
	graph G(filename.data());
	louvain L(G);
	EXPECT_GT(L.getCommunitiesCount(), 0) << "Count of communities = 0. File: " << filename;
}
TEST_P(LouvainFileTests, ModularityValidation) {
	graph G(filename.data());
	louvain L(G);
	std::vector<int> partition = L.getPartition();
	ASSERT_EQ(-1<=L.getModularity(G,partition)<=1, 1) << "Modularity isn't in range [-1;1]. File: " << filename;
}
TEST_P(LouvainFileTests, CheckingThatCommunitiesMatecheWithPartition) { //проверка, что содержимое communities совпадает с тем, какое разбиение получается в result
	graph G(filename.data());
	louvain L(G);
	std::vector<int> partition = L.getPartition();
	std::vector<std::unordered_set<int>> communities = L.getCommunities();
	for (int c = 0; c < L.getCommunitiesCount(); c++) { 
		for (auto it = communities[c].begin(); it != communities[c].end(); it++) {
			int v = (*it);
			EXPECT_EQ(c,partition[v]);
			if( partition[v] != c )std::cout << "mistake " << c << " " << partition[v] << "\n ";
		}
	}
}

//можно добавить тесты на проверку результатов с результатами из python
INSTANTIATE_TEST_CASE_P( GraphFiles,
	LouvainFileTests,
	::testing::ValuesIn(GetTestFiles()),
	[](const ::testing::TestParamInfo<std::string>& info) {
		// генерируем читаемые имена тестов
		std::string name = "Graph" + std::to_string(info.index);
		return name;
	}
);

TEST(louvainAlgorithm, FictitiousTest)
{
	EXPECT_EQ(0, 0);
}

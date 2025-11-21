#pragma once
#include <map>
#include <set>
#include <unordered_set>
#include "graph.h"

class louvain_new {
	int N = 0;
	int M = 0;
	std::vector<int> teck_partition;
	std::vector<std::unordered_set<int>> teck_communities;
	std::vector<std::unordered_set<int>> communities;
	std::vector<int> result;
	
	graph g;
	int teck_community_count = 0;
	std::vector<double> d;
	std::vector<double> in;
	std::vector<double> tot;
public:
	int getCommunitiesCount();
	std::vector<int> getPartition();
	void printPartition();
	void printTeckPartition();
	void printInTot(int n);
	void printCommunities();
	void printDebugInfo();
	void printTeckCommunities();
	void printResultCommunities();
	void printD();

	void reculculate(const graph& g, const std::vector<int>& partition);
	void setSinglePartition(int n);
	static float getModularity(const graph& g, const std::vector<int>& partition);
	double d_i_C(const graph& g, const int& v, const std::vector<int>& partition, const int& C);
	double d_i(const graph& g, const int& v);
	double getGain(const graph& g, const int& v, const std::vector<int>& partition, const int& C);
	void remove(int v, int C, const graph& g, std::vector<int>& partition);
	void insert(int v, int C, const graph& g, std::vector<int>& partition);
	std::pair<float, int> getBestDelta(const graph& g, const int& v, std::vector<int>& partition);

	graph aggregateGraph(graph& g, std::vector<int>& partition);
	void aggregateGraphOptimized(graph& g, std::vector<int>& partition);
	void moveNodes(graph& g, std::vector<int>& partition);
	louvain_new(const graph& G);
};



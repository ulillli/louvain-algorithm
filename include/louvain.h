#pragma once
#include <map>
#include <set>
#include <unordered_set>
#include "graph.h"

class louvain {
	int N = 0;
	float M = 0;
	float first_modularity = 0.0;
	std::vector<int> teck_partition;
	std::vector<std::unordered_set<int>> teck_communities;
	std::vector<std::unordered_set<int>> communities;
	std::vector<int> result;
	
	graph g;
	int teck_community_count = 0;
	std::vector<double> d;
	std::vector<double> in;
	std::vector<double> tot;
	std::vector<double> in_G;
	std::vector<double> tot_G;
public:
	int getCommunitiesCount();
	std::vector<int> getPartition();
	std::vector<std::unordered_set<int>> getCommunities();
	float getFirstModularity();
	void printPartition();
	void printTeckPartition();
	void printInTot(int n);
	void printCommunities();
	void printDebugInfo();
	void printTeckCommunities();
	void printResultCommunities();
	void printD();

	void inizialization(const graph& G);
	void reculculate(const graph& g, const std::vector<int>& partition);
	void setSinglePartition(int n);
	float getModularity(const graph& g, const std::vector<int>& partition);
	float getModularityOptimized(const graph& g, const std::vector<int>& partition, bool flag);
	double d_i_C(const graph& g, const int& v, const std::vector<int>& partition, const int& C);
	double d_i(const graph& g, const int& v);
	double getGain(const graph& g, const int& v, const std::vector<int>& partition, const int& C);
	void remove(int v, int C, const graph& g, std::vector<int>& partition);
	void insert(int v, int C, const graph& g, std::vector<int>& partition);
	std::pair<float, int> getBestDelta(const graph& g, const int& v, std::vector<int>& partition);

	void aggregateGraphOptimized(graph& g, std::vector<int>& partition);
	void moveNodes(graph& g, std::vector<int>& partition);
	louvain(const graph& G);
};



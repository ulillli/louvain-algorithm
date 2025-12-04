#pragma once
#include <map>
#include <set>
#include <unordered_set>
#include "graph_vec.h"

class louvain_vec {
	int N = 0;
	float M = 0;
	float first_modularity = 0.0;
	std::vector<int> teck_partition;
	std::vector<std::unordered_set<int>> teck_communities;
	std::vector<std::unordered_set<int>> communities;
	std::vector<int> result;
	
	graph_vec g;
	int teck_community_count = 0;
	std::vector<double> d;
	std::vector<std::unordered_map<int, double>> d_i_c; //номер сообщества, суммарный вес ребер-соседей, входящих в сообщество 
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

	void inizialization(const graph_vec& G);
	void reculculate(const graph_vec& g, const std::vector<int>& partition);
	void setSinglePartition(int n);
	float getModularity(const graph_vec& g, const std::vector<int>& partition);
	double getModularityOptimized(const graph_vec& g, const std::vector<int>& partition, bool flag);
	double d_i_C(const graph_vec& g, const int& v, const std::vector<int>& partition, const int& C);
	double d_i(const graph_vec& g, const int& v);
	double getGain(const graph_vec& g, const int& v, const std::vector<int>& partition, const int& C);
	void remove(int v, int C, const graph_vec& g, std::vector<int>& partition);
	void insert(int v, int C, const graph_vec& g, std::vector<int>& partition);
	std::pair<float, int> getBestDelta(const graph_vec& g, const int& v, std::vector<int>& partition);

	void aggregateGraphOptimized(graph_vec& g, std::vector<int>& partition);
	void moveNodes(graph_vec& g, std::vector<int>& partition);
	louvain_vec(const graph_vec& G);
};



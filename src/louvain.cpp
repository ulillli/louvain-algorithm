#pragma once
#include <map>
#include <set>
#include <unordered_set>
#include <chrono>
#include "louvain.h"

int louvain_new::getCommunitiesCount() { return teck_community_count; }
void louvain_new::printPartition() {
	for (int i = 0; i < N; i++) std::cout << result[i] << " ";
	std::cout << std::endl;
}
void louvain_new::printTeckPartition() {
	for (int i = 0; i < teck_community_count; i++) std::cout << teck_partition[i] << " ";
	std::cout << std::endl;
}
void louvain_new::printInTot(int n) {
	std::cout << "in: ";
	for (int i = 0; i < n; i++) std::cout << in[i] << " ";
	std::cout << '\n';
	std::cout << "tot: ";
	for (int i = 0; i < n; i++) std::cout << tot[i] << " ";
	std::cout << '\n';
}
void louvain_new::printCommunities() {
	for (int c = 0; c < teck_community_count; c++) { 
		if (!communities[c].empty()) { 
			std::cout << c << ": ";
			for (auto it = communities[c].begin(); it != communities[c].end(); it++) {
				int v = (*it); 
				std::cout << v << " ";
			}
			std::cout << '\n';
		}
	}
}
void louvain_new::printTeckCommunities() {
	for (int c = 0; c < teck_communities.size(); c++) {
		if (!teck_communities[c].empty()) {
			std::cout << c << ": ";
			for (auto it = teck_communities[c].begin(); it != teck_communities[c].end(); it++) {
				int v = (*it);
				std::cout << v << " ";
			}
			std::cout << '\n';
		}
	}
}
void louvain_new::printDebugInfo() {
	std::cout << "partition: ";
	printTeckPartition();
	std::cout << "result: ";
	printPartition();
	printCommunities();
}
void louvain_new::reculculate(const graph& g, const std::vector<int>& partition) {
	int n = g.getVertexCount();
	in = std::vector<int>(teck_community_count, 0);
	tot = std::vector<int>(teck_community_count, 0);
	for (int v = 0; v < n; v++) {
		int v_community = partition[v];
		tot[v_community] += g[v].size();
		for (int j = 0; j < g[v].size(); j++) {
			int neighbour = g[v][j];
			int neighbour_community = partition[neighbour];
			if (neighbour_community == v_community) in[neighbour_community]++;
		}
	}
}
void louvain_new::setSinglePartition(int n) {
	teck_partition = std::vector<int>(teck_community_count);
	teck_communities = std::vector<std::unordered_set<int>>(teck_community_count);
	for (int i = 0; i < teck_community_count; i++) {
		teck_partition[i] = i;
		teck_communities[i].insert(i);
	}
	reculculate(g, teck_partition);
}
int louvain_new::d_i_C(const graph& g, const int& v, const std::vector<int>& partition, const int& C) {
	int result = 0;
	for (int j = 0; j < g[v].size(); j++) {
		int neighbour = g[v][j];
		int neighbour_community = partition[neighbour];
		if (neighbour_community == C) {
			result++;
		}
	}
	return result;
}
float louvain_new::getGain(const graph & g, const int& v, const std::vector<int>& partition, const int& C) {
	int m = g.getEdgeCount();
	float gain11 = (in[C] + 2 * d_i_C(g, v, partition, C)) / (2.0f * m);
	float gain12 = ((tot[C] + g[v].size())/(2.0f * m)) * ((tot[C] + g[v].size()) / (2.0f * m));
	float gain21 = in[C] / (2.0f * m);
	float gain22 = (tot[C] / (2.0f * m)) * (tot[C] / (2.0f * m));
	float gain23 = (g[v].size() / (2.0f * m)) * (g[v].size() / (2.0f * m));
	return (gain11 - gain12) - (gain21 - gain22 - gain23);
}
void louvain_new::remove(int v, int C,const graph & g, std::vector<int>& partition) {
	in[C] = in[C] - 2 * d_i_C(g, v, partition, C);
	tot[C] = tot[C] - g[v].size(); 	
}
void louvain_new::insert(int v, int C, const graph& g, std::vector<int>& partition) {
	in[C] = in[C] + 2 * d_i_C(g, v, partition, C);
	tot[C] = tot[C] + g[v].size();
}
std::pair<float, int> louvain_new::getBestDelta(const graph& g, const int& v, std::vector<int>& partition) {
		int m = g.getEdgeCount();
		int v_community = partition[v];
		remove(v, v_community, g, partition);
		float best_gain = getGain(g, v, partition, v_community);
		int best_community = v_community;
		for (int j = 0; j < g[v].size(); j++) {
			int neighbour = g[v][j];
			int neighbour_community = partition[neighbour];
			if (neighbour_community != v_community) {
				float gain = getGain(g, v, partition, neighbour_community);
				if (gain > best_gain) {
					best_gain = gain;
					best_community = neighbour_community;
				}
			}
		}
		insert(v, best_community, g, partition);
		return { best_gain,best_community };
}
void louvain_new::aggregateGraphOptimized(graph& g, std::vector<int>& partition) {
		std::cout << "Before AGGREGETION\n";
		printDebugInfo();
		printInTot(teck_community_count);
		std::cout << "Teck communities\n";
		printTeckCommunities();
		std::set<std::pair<int, int>> new_edges;
		int edges_count = 0;
		int n = g.getVertexCount();
		std::vector<std::unordered_set<int>> communities_ = std::vector<std::unordered_set<int>>();
		for (int c = 0; c < n; c++) {
			if (!teck_communities[c].empty()) communities_.push_back(teck_communities[c]);
		}
		teck_communities = communities_;
		if (teck_communities.size() == teck_community_count) return;
		communities_ = std::vector<std::unordered_set<int>>(teck_communities.size());
		for (int c_new = 0; c_new < teck_communities.size(); c_new++) {
			for (auto it = teck_communities[c_new].begin(); it != teck_communities[c_new].end(); it++) {
				int c_old = (*it); //это получается номер старого коммьюнити 
				partition[c_old] = c_new;
				for (auto it = communities[c_old].begin(); it != communities[c_old].end(); it++) {
					int v = (*it);
					result[v] = c_new;
					communities_[c_new].insert(v);
				}
			}
		}
		std::cout << "Teck communities after changes\n";
		printTeckCommunities();
		printTeckPartition();
		printPartition();
		communities = communities_;
		teck_community_count = teck_communities.size();
		for (int v = 0; v < n; v++) {
			int v_community = partition[v];
			for (int j = 0; j < g[v].size(); j++) {
				int neighbour = g[v][j];
				int neighbour_community = partition[neighbour];
				if (v_community != neighbour_community) {
					if (new_edges.find({ v_community,neighbour_community }) == new_edges.end() && new_edges.find({ neighbour_community,v_community }) == new_edges.end()) {
						new_edges.insert({ partition[v],neighbour_community });
					}
				}
			}
		}
		n = teck_community_count;
		int m = new_edges.size();
		std::vector<std::vector<int>> adj(n);
		for (auto it = new_edges.begin(); it != new_edges.end(); it++) {
			int x = (*it).first;
			int y = (*it).second;
			adj[x].push_back(y);
			adj[y].push_back(x);
		}
		g=graph(adj, n, m);
		setSinglePartition(teck_community_count);
		std::cout << "After AGGREGETION\n";
		printDebugInfo();
		std::cout << "Teck communities after AGGREGETION\n";
		printTeckCommunities();
		printInTot(teck_community_count);
	}
void louvain_new::moveNodes(graph& g, std::vector<int>& partition) {
	//std::cout << "Before MOVING_NODES\n";
	//printDebugInfo();
	int n = g.getVertexCount();
	float current_modularity = getModularity(g, partition);
	//std::cout << "Current modularity: " << current_modularity << '\n';
	//printInTot(n);
	float old_modularity = -1.0f;
	do {
		old_modularity = current_modularity;
		for (int v = 0; v < n; v++) {
			int v_community = partition[v];
			std::pair<float, int> best_delta = getBestDelta(g, v, partition);
			if (best_delta.first > 0.0 && best_delta.second != v_community) {
				partition[v] = best_delta.second;
				teck_communities[best_delta.second].insert(v);
				teck_communities[v_community].erase(v);
				//std::cout << v <<  " was moved to " << best_delta.second << " with best_delta_modularity " << best_delta.first << '\n';
				//std::cout << "Current modularity: " << getModularity(g,partition) << '\n';
			}
		}
		current_modularity = getModularity(g, partition);
		//printTeckPartition();
	} while (current_modularity > old_modularity);
	std::cout << std::endl;
	//printInTot(n);
	//std::cout << "After MOVING_NODES\n";
	//printDebugInfo();
}
louvain_new::louvain_new(const graph& G) {
		N = G.getVertexCount();
		M = G.getEdgeCount();
		g = G;
		teck_community_count = N;
		teck_partition = std::vector<int>(N);
		result = std::vector<int>(N);
		setSinglePartition(teck_community_count);
		communities = std::vector<std::unordered_set<int>>(teck_community_count);
		for (int i = 0; i < N; i++) { 
			communities[i].insert(i);
			result[i] = i;
		}
		std::cout << "Current modularity: " << getModularity(g, result) << '\n';
		bool flag = 0;
		auto start = std::chrono::steady_clock::now();
		do {
			int old_communities_count = teck_community_count;
			moveNodes(g, teck_partition);
			aggregateGraphOptimized(g, teck_partition);
			flag = old_communities_count == teck_community_count; 
			std::cout << "Communities count = " << teck_community_count << std::endl;
		} while (!flag);
		
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> time = end - start;
		std::cout << "Current modularity: " << getModularity(g, result) << '\n';
		std::cout << time.count() << std::endl;
	}

float louvain_new::getModularity(const graph& g, const std::vector<int>& partition) {
	int community_count = 0;
	int n = g.getVertexCount(), m = g.getEdgeCount();
	for (int i = 0; i < n; i++) {
		if (partition[i] + 1 > community_count) community_count = partition[i] + 1;
	}
	std::vector<int> in = std::vector<int>(community_count, 0);
	std::vector<int> tot = std::vector<int>(community_count, 0);
	for (int v = 0; v < n; v++) {
		int v_community = partition[v]; 
		tot[v_community] += g[v].size();
		for (int j = 0; j < g[v].size(); j++) { 
			int neighbour = g[v][j];
			int neighbour_community = partition[neighbour];
			if (neighbour_community == v_community) in[partition[neighbour]]++;
		}
	}
	float result = 0;
	for (int i = 0; i < community_count; i++) {
		result += in[i] / (2.0f * m) - (tot[i] / (2.0f * m)) * (tot[i] / (2.0f * m));
	}
	return result;
}

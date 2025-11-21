#pragma once
#include <chrono>
#include "louvain.h"

int louvain_new::getCommunitiesCount() { return teck_community_count; }
std::vector<int> louvain_new::getPartition() {
	return result;
}
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
	std::cout << "[";
	for (int c = 0; c < teck_communities.size(); c++) {
		if (!teck_communities[c].empty()) {
			std::cout << "'";
			auto end = teck_communities[c].end();
			end--;
			for (auto it = teck_communities[c].begin(); it != end; it++) {
				int v = (*it);
				std::cout << v << ", ";
			}
			if(c == teck_communities.size()-1) std::cout << *end << "'";
			else std::cout << *end << "',";
		}
	}
	std::cout << "]\n";
}
void louvain_new::printResultCommunities() {
	std::cout << "[";
	for (int c = 0; c < teck_communities.size(); c++) {
		
			std::cout << "'";
			auto end = communities[c].end();
			end--;
			for (auto it = communities[c].begin(); it != end; it++) {
				int v = (*it);
				std::cout << v << ", ";
			}
			if (c == communities.size() - 1) std::cout << *end << "'";
			else std::cout << *end << "',";
	}
	std::cout << "]\n";
}
void louvain_new::printDebugInfo() {
	std::cout << "partition: ";
	printTeckPartition();
	std::cout << "result: ";
	printPartition();
	printCommunities();
}
void louvain_new::printD() {
	for (int i = 0; i < g.getVertexCount(); i++) std::cout << d[i] << " ";
	std::cout << std::endl;
}

void louvain_new::reculculate(const graph& g, const std::vector<int>& partition) {
	int n = g.getVertexCount();
	d = std::vector<double>(n, 0);
	in = std::vector<double>(teck_community_count, 0);
	tot = std::vector<double>(teck_community_count, 0);
	for (int v = 0; v < n; v++) {
		int v_community = partition[v];
		for (auto it = g[v].begin(); it != g[v].end(); it++) {
			int neighbour = (*it).first;
			int neighbour_community = partition[neighbour];
			tot[v_community] += (*it).second;
			d[v]+= (*it).second;
			if (neighbour_community == v_community) in[neighbour_community] += (*it).second;
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
double louvain_new::d_i_C(const graph& g, const int& v, const std::vector<int>& partition, const int& C) {
	double result = 0;
	for (auto it = g[v].begin(); it != g[v].end(); it++) {
		int neighbour = (*it).first;
		int neighbour_community = partition[neighbour];
		if (neighbour_community == C && v != neighbour) { //тут не нужно учитывать петли
			result += (*it).second;
		}
	}
	return result;
}
double louvain_new::d_i(const graph& g, const int& v) {
	double result = 0;
	for (auto it = g[v].begin(); it != g[v].end(); it++) {
		result += (*it).second; 
	}
	return result;
}
double louvain_new::getGain(const graph& g, const int& v, const std::vector<int>& partition, const int& C) {
	int m = g.getEdgeCount();
	double gain11 = (in[C] + 2 * d_i_C(g, v, partition, C)) / (2.0f * m);
	double gain12 = ((tot[C] + d[v]) / (2.0f * m)) * ((tot[C] + d[v]) / (2.0f * m));
	double gain21 = in[C] / (2.0f * m);
	double gain22 = (tot[C] / (2.0f * m)) * (tot[C] / (2.0f * m));
	double gain23 = (d[v] / (2.0f * m)) * (d[v] / (2.0f * m));
	return (gain11 - gain12) - (gain21 - gain22 - gain23);
}
void louvain_new::remove(int v, int C, const graph& g, std::vector<int>& partition) {
	in[C] = in[C] - 2 * d_i_C(g, v, partition, C) - g.getWeightOfLoop(v);
	tot[C] = tot[C] - d[v];
}
void louvain_new::insert(int v, int C, const graph& g, std::vector<int>& partition) {
	in[C] = in[C] + 2 * d_i_C(g, v, partition, C) + g.getWeightOfLoop(v);
	tot[C] = tot[C] + d[v];
}
std::pair<float, int> louvain_new::getBestDelta(const graph& g, const int& v, std::vector<int>& partition) {
	int m = g.getEdgeCount();
	int v_community = partition[v];
	
	remove(v, v_community, g, partition);
	
	double best_gain = getGain(g, v, partition, v_community);
	//std::cout << v << " is moving to " << v_community << " with delta " << best_gain << std::endl;
	int best_community = v_community;
	for (auto it = g[v].begin(); it != g[v].end(); it++) {
		int neighbour = (*it).first;
		int neighbour_community = partition[neighbour];
		if (neighbour_community != v_community) {
			double gain = getGain(g, v, partition, neighbour_community);
			//std::cout << v << " is moving to " << neighbour_community << " with delta " << gain << std::endl;
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
			for (auto it_ = communities[c_old].begin(); it_ != communities[c_old].end(); it_++) {
				int v = (*it_);
				result[v] = c_new;
				communities_[c_new].insert(v);
			}
		}
	}
	communities = communities_;
	teck_community_count = teck_communities.size();
	std::vector<std::unordered_map<int, double>> adj(teck_community_count);
	for (int v = 0; v < n; v++) {
		int v_community = partition[v];
		for (auto it = g[v].begin(); it != g[v].end(); it++) {
			int neighbour = (*it).first;
			int neighbour_community = partition[neighbour];
			adj[v_community][neighbour_community] += (*it).second;
			/*if (v_community != neighbour_community) {
				adj[v_community][neighbour_community] += (*it).second;
			}
			else {
				adj[v_community][v_community] += (*it).second; 
			}*/
		}
	}
	n = teck_community_count;
	float m = 0;
	for (int v = 0; v < n; v++) {
		for (auto it = adj[v].begin(); it != adj[v].end(); it++) {
			int u = it->first;
			double weight = it->second;
			m += weight;
		}
	}
	std::cout << m << " " << m/2 << std::endl;
	g = graph(adj, n, m/2);
	setSinglePartition(teck_community_count);
}
void louvain_new::moveNodes(graph& g, std::vector<int>& partition) {

	int n = g.getVertexCount();
	float current_modularity = getModularity(g, partition);

	float old_modularity = -1.0f;
	do {
		old_modularity = current_modularity;
		for (int v = 0; v < n; v++) {
			int v_community = partition[v];
			//if (v % 1000 == 0) std::cout << v << std::endl;
			std::pair<float, int> best_delta = getBestDelta(g, v, partition);
			if (best_delta.first > 0.0 && best_delta.second != v_community) {
				float tmp = getModularity(g, partition);
				partition[v] = best_delta.second;
				teck_communities[best_delta.second].insert(v);
				teck_communities[v_community].erase(v);
				//std::cout << v <<  " was moved to " << best_delta.second << " with best_delta_modularity " << best_delta.first << "\n";
			}
		}
		current_modularity = getModularity(g, partition);
		std::cout << "Current modularity in moving nodes: " << current_modularity << '\n';
		//g.printAdjList();
	} while (current_modularity > old_modularity);
	
}
louvain_new::louvain_new(const graph& G) {
	N = G.getVertexCount();
	M = G.getEdgeCount();
	g = G;
	teck_community_count = N;
	teck_partition = std::vector<int>(N);
	result = std::vector<int>(N);
	setSinglePartition(teck_community_count);
	reculculate(g, teck_partition);
	communities = std::vector<std::unordered_set<int>>(teck_community_count);
	for (int i = 0; i < N; i++) {
		communities[i].insert(i);
		result[i] = i;
	}
	bool flag = false;
	auto start = std::chrono::steady_clock::now();
	float old = getModularity(g, teck_partition);
	do {
		std::cout << "Weight of all edges(m) = " << g.getEdgeCount() << std::endl;
		moveNodes(g, teck_partition);
		float curr = getModularity(g, teck_partition);
		if (curr > old) { 
			old = curr;
			flag = true;
			aggregateGraphOptimized(g, teck_partition);
		}
		else flag = false;
	} while (flag);

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<float> time = end - start;
	/*for (int c = 0; c < teck_community_count; c++) { //добавить такой тест 
			for (auto it = communities[c].begin(); it != communities[c].end(); it++) {
				int v = (*it);
				if (c != result[v]) std::cout << "mistake " << c << " " << result[v] << "\n ";
			}
	}*/
	std::cout << "Modularity: " << getModularity(G, result) << '\n';
	std::cout << time.count() << std::endl;
}
float louvain_new::getModularity(const graph& g, const std::vector<int>& partition) {
	int community_count = 0;
	int n = g.getVertexCount(), m = g.getEdgeCount();
	//std::cout <<" in modularity " << m << std::endl;
	for (int i = 0; i < n; i++) {
		if (partition[i] + 1 > community_count) community_count = partition[i] + 1;
	}
	//std::cout << "in modularity community_count " << community_count << std::endl;

	std::vector<int> in = std::vector<int>(community_count, 0);
	std::vector<int> tot = std::vector<int>(community_count, 0);
	for (int v = 0; v < n; v++) {
		int v_community = partition[v];
		for (auto it = g[v].begin(); it != g[v].end(); it++) {
			int neighbour = (*it).first;
			int neighbour_community = partition[neighbour];
			tot[v_community] += (*it).second;
			if (neighbour_community == v_community) { 
				in[neighbour_community] += (*it).second; 
			}
		}
	}
	
	float result = 0;
	for (int i = 0; i < community_count; i++) {
		result += in[i] / (2.0f * m) - (tot[i] / (2.0f * m)) * (tot[i] / (2.0f * m));
	}
	//std::cout << "Modularity: " << result <<" for n = " << n << '\n';
	return result;
}

#pragma once
#include <chrono>
#include <iomanip>
#include "louvain_vec.h"

int louvain_vec::getCommunitiesCount() { return teck_community_count; }
std::vector<std::unordered_set<int>> louvain_vec::getCommunities() { return communities; }
float louvain_vec::getFirstModularity() { return first_modularity; }
std::vector<int> louvain_vec::getPartition() {
	return result;
}
void louvain_vec::printPartition() {
	for (int i = 0; i < N; i++) std::cout << result[i] << " ";
	std::cout << std::endl;
}
void louvain_vec::printTeckPartition() {
	for (int i = 0; i < teck_community_count; i++) std::cout << teck_partition[i] << " ";
	std::cout << std::endl;
}
void louvain_vec::printInTot(int n) {
	std::cout << "in: ";
	for (int i = 0; i < n; i++) std::cout << in[i] << " ";
	std::cout << '\n';
	std::cout << "tot: ";
	for (int i = 0; i < n; i++) std::cout << tot[i] << " ";
	std::cout << '\n';
}
void louvain_vec::printCommunities() {
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
void louvain_vec::printTeckCommunities() {
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
void louvain_vec::printResultCommunities() {
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
void louvain_vec::printDebugInfo() {
	std::cout << "partition: ";
	printTeckPartition();
	std::cout << "result: ";
	printPartition();
	printCommunities();
}
void louvain_vec::printD() {
	for (int i = 0; i < g.getVertexCount(); i++) std::cout << d[i] << " ";
	std::cout << std::endl;
}

void louvain_vec::reculculate(const graph_vec& g, const std::vector<int>& partition) {
	int n = g.getVertexCount();
	d = std::vector<double>(n, 0);
	//d_i_c = std::vector<std::unordered_map<int,double>>(n);
	in = std::vector<double>(teck_community_count, 0);
	tot = std::vector<double>(teck_community_count, 0);
	for (int v = 0; v < n; v++) {
		int v_community = partition[v];
		for (auto it = g[v].begin(); it != g[v].end(); it++) {
			int neighbour = (*it).first;
			int neighbour_community = partition[neighbour];
			tot[v_community] += (*it).second;
			d[v]+= (*it).second;
			//if(v!= neighbour) d_i_c[v][neighbour_community] += (*it).second;
			if (neighbour_community == v_community) in[neighbour_community] += (*it).second;
		}
	}
}

void louvain_vec::setSinglePartition(int n) {
	teck_partition = std::vector<int>(teck_community_count);
	teck_communities = std::vector<std::unordered_set<int>>(teck_community_count);
	for (int i = 0; i < teck_community_count; i++) {
		teck_partition[i] = i;
		teck_communities[i].insert(i);
	}
	reculculate(g, teck_partition);
}
double louvain_vec::d_i_C(const graph_vec& g, const int& v, const std::vector<int>& partition, const int& C) {
	double result = 0;
	for (auto it = g[v].begin(); it != g[v].end(); it++) {
		int neighbour = (*it).first;
		int neighbour_community = partition[neighbour];
		if (neighbour_community == C && v != neighbour) { 
			result += (*it).second;
		}
	}
	return result;
}
double louvain_vec::getGain(const graph_vec& g, const int& v, const std::vector<int>& partition, const int& C) {
	int m = g.getEdgeCount();
	double gain11 = (in[C] + 2 * d_i_C(g, v, partition, C)) / (2.0 * m);
	//auto it = d_i_c[v].find(C);
	//double gain11 = (in[C] + 2 *((it != d_i_c[v].end()) ? it->second : 0.0))/ (2.0 * m);
	double gain12 = ((tot[C] + d[v]) / (2.0 * m)) * ((tot[C] + d[v]) / (2.0 * m));
	double gain21 = in[C] / (2.0 * m);
	double gain22 = (tot[C] / (2.0 * m)) * (tot[C] / (2.0 * m));
	double gain23 = (d[v] / (2.0 * m)) * (d[v] / (2.0 * m));
	return (gain11 - gain12) - (gain21 - gain22 - gain23);
}
void louvain_vec::remove(int v, int C, const graph_vec& g, std::vector<int>& partition) {
	in[C] = in[C] - 2 * d_i_C(g, v, partition, C) - g.getWeightOfLoop(v);
	/*auto it = d_i_c[v].find(C);
	in[C] = in[C] - 2 * ((it != d_i_c[v].end()) ? it->second : 0.0) - g.getWeightOfLoop(v);*/
	tot[C] = tot[C] - d[v];
}
void louvain_vec::insert(int v, int C, const graph_vec& g, std::vector<int>& partition) {
	in[C] = in[C] + 2 * d_i_C(g, v, partition, C) + g.getWeightOfLoop(v);
	/*auto it = d_i_c[v].find(C);
	in[C] = in[C] + 2 * ((it != d_i_c[v].end()) ? it->second : 0.0) + g.getWeightOfLoop(v);*/
	tot[C] = tot[C] + d[v];
}
std::pair<float, int> louvain_vec::getBestDelta(const graph_vec& g, const int& v, std::vector<int>& partition) {
	int m = g.getEdgeCount();
	int v_community = partition[v];
	remove(v, v_community,g, partition);
	
	double best_gain = getGain(g, v, partition, v_community);
	int best_community = v_community;
	std::set<int> C;
	for (int j = 0; j < g[v].size(); j++) {
		int neighbour = g[v][j].first;
		int neighbour_community = partition[neighbour];
		if (neighbour_community != v_community) C.insert(neighbour_community);
	}
	for (auto it = C.begin(); it != C.end(); it++) {
		int community = *it;
		double gain = getGain(g, v, partition, community);
		if (gain > best_gain) {
			best_gain = gain;
			best_community = community;
		}
	}
	//for (int j = 0; j < g[v].size(); j++) {
	//	int neighbour = g[v][j].first;
	//	int neighbour_community = partition[neighbour];
	//	if (neighbour_community != v_community) {
	//		double gain = getGain(g, v, partition, neighbour_community);
	//		//std::cout << v << " is moving to " << neighbour_community << " with delta " << gain << std::endl;
	//		if (gain > best_gain) {
	//			best_gain = gain;
	//			best_community = neighbour_community;
	//		}
	//	}
	//}
	insert(v, best_community, g, partition);
	return { best_gain,best_community };
}
void louvain_vec::aggregateGraphOptimized(graph_vec& g, std::vector<int>& partition) {
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
	std::vector<std::vector<std::pair<int, double>>> adj(teck_community_count);
	std::vector<double> loops(teck_community_count,0.0);
	for (int v = 0; v < n; v++) {
		int v_community = partition[v];
		for (auto it = g[v].begin(); it != g[v].end(); it++) {
			int neighbour = (*it).first;
			int neighbour_community = partition[neighbour];
			bool flag = 0;
			for (int j = 0; j < adj[v_community].size(); j++) {
				if (adj[v_community][j].first == neighbour_community) {
					flag = 1;
					adj[v_community][j].second += (*it).second;
				}
			}
			if(!flag)adj[v_community].push_back({ neighbour_community,(*it).second });
			if(v_community == neighbour_community)  loops[v_community] += (*it).second;
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
	g = graph_vec(adj,loops, n, m/2);
	setSinglePartition(teck_community_count);
}
void louvain_vec::moveNodes(graph_vec& g, std::vector<int>& partition) {
	int n = g.getVertexCount();
	double current_modularity = getModularityOptimized(g, partition, 1);
	double old_modularity = -1.0f;
	do {
		old_modularity = current_modularity;
		for (int v = 0; v < n; v++) {
			int v_community = partition[v];
			if (v!=0 && v % 100000 == 0) std::cout << v << std::endl;
			std::pair<float, int> best_delta = getBestDelta(g, v, partition);
			if (best_delta.first > 0.0 && best_delta.second != v_community) {
				//double tmp = getModularityOptimized(g, partition, 1);

				//for (int i = 0; i < g[v].size(); i++) {
				//	if (g[v][i].first != v) {
				//		//if (partition[g[v][i].first] == v_community) d_i_c[v][v_community] += g[v][i].second;
				//		d_i_c[g[v][i].first][v_community] -= g[v][i].second;
				//		d_i_c[g[v][i].first][best_delta.second] += g[v][i].second;
				//		if (d_i_c[g[v][i].first][v_community] == 0.0) d_i_c[g[v][i].first].erase(v_community);
				//	}

				//}
				partition[v] = best_delta.second;
				teck_communities[best_delta.second].insert(v);
				teck_communities[v_community].erase(v);
			}
		}
		current_modularity = getModularityOptimized(g, partition, 1);
		std::cout << std::setprecision(20) << current_modularity << std::endl;
	} while (current_modularity > old_modularity);
}
void louvain_vec::inizialization(const graph_vec& G) {
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
	in_G = in;
	tot_G = tot;
}
louvain_vec::louvain_vec(const graph_vec& G) {
	inizialization(G);
	bool flag = false;
	auto start = std::chrono::steady_clock::now();
	double old = getModularityOptimized(g, teck_partition,0);
	first_modularity = old;
	std::cout << "Modularity: " << old << '\n';
	do {
		moveNodes(g, teck_partition);
		double curr = getModularityOptimized(g, teck_partition,1);
		if (curr > old) { 
			old = curr;
			flag = true;
			aggregateGraphOptimized(g, teck_partition);
		}
		else flag = false;
	} while (flag);

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<float> time = end - start;
	std::cout << "Modularity: " << getModularity(G, result) << '\n';
	std::cout << "Count of communities: " << getCommunitiesCount() << std::endl;
	std::cout << time.count() << std::endl;
}
double louvain_vec::getModularityOptimized(const graph_vec & g, const std::vector<int>& partition, bool flag) {
	int community_count = 0;
	int n = g.getVertexCount(), m = g.getEdgeCount();
	double tmp = 1 / (2.0 * m);
	community_count = teck_community_count;
	double result = 0;
	if (flag == 0) {
		for (int i = 0; i < community_count; i++) {
			result += in_G[i] * tmp - (tot_G[i]*tmp) * (tot_G[i] * tmp);
		}
	}
	else {
		for (int i = 0; i < community_count; i++) {
			result += in[i] *tmp - (tot[i]*tmp) * (tot[i] *tmp);
		}
	}
	return result;
}
float louvain_vec::getModularity(const graph_vec& g, const std::vector<int>& partition) {
	int community_count = 0;
	int n = g.getVertexCount(), m = g.getEdgeCount();
	for (int i = 0; i < n; i++) {
		if (partition[i] + 1 > community_count) community_count = partition[i] + 1;
	}
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
	return result;
}
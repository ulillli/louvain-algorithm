#pragma once
#include <fstream>
#include <iostream>
#include "graph_vec.h"

graph_vec::graph_vec(std::vector<std::vector<std::pair<int, double>>> v, std::vector <double> l, int n, int m) :N(n), M(m), adj(v), loops(l) {}
graph_vec::graph_vec(const char* filename, int flag) { //если flag = 0, то в файле нет весов ребер, иначе  - есть
	std::ifstream file(filename, std::ios::in);
	file >> N >> M;
	int x, y;
	adj = std::vector<std::vector<std::pair<int, double>>>(N);
	loops = std::vector<double>(N, 0.0);
	exist = std::vector <std::vector <int>>(N);
	std::vector<std::unordered_map<int, double>> adj_tmp = std::vector<std::unordered_map<int, double>>(N);
	if (flag == 0) { 
		int count = 0;
		while (file >> x >> y) {
			if (x != y) { 
				auto result1=adj_tmp[x].insert({ y,1.0 });
				auto result2= adj_tmp[y].insert({ x,1.0 });
				if (result1.second) count++;
				if (result2.second) count++;
			}
		}
		//std::cout << count << std::endl;
		for (int i = 0; i < N; i++) { //добавляем петли сами с 0-ым весом
			adj_tmp[i].insert({ i,0.0 });
		}
		double m = 0;
		for (int v = 0; v < N; v++) {
			for (auto it = adj_tmp[v].begin(); it != adj_tmp[v].end(); it++) {
				adj[v].push_back({ (*it).first,(*it).second });
				m += (*it).second;
			}
		}
		//std::cout << m << std::endl;
		M = m / 2.0;
	}
}
std::vector<std::pair<int, double>>& graph_vec::operator[](const int& i) {
	return adj[i];
}
const std::vector<std::pair<int, double>>& graph_vec::operator[](const int& i) const {
	return adj[i];
}
int graph_vec::getVertexCount() const { return N; }
int graph_vec::getEdgeCount() const { return M; }
void graph_vec::printAdjList() const {
	for (int v = 0; v < adj.size(); v++) {
		std::cout << v << ": ";
		for (int j = 0; j<adj[v].size(); j++) {
			std::cout << "{" << adj[v][j].first << "," << adj[v][j].second << "} ";
		}
		std::cout << std::endl;
	}
}
void graph_vec::printNeighbours(const int& v) const {
	std::cout << v << ": ";
	for (auto it = adj[v].begin(); it != adj[v].end(); it++) {
		std::cout << (*it).first << " ";
	}
	std::cout << std::endl;
}
double graph_vec::getWeightOfLoop(const int& v) const {
	return loops[v];
}

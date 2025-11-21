#pragma once
#include <fstream>
#include <iostream>
#include "graph.h"

graph::graph(std::vector<std::unordered_map<int, double>> v, int n, int m) :N(n), M(m), adj(v) {}
graph::graph(const char* filename, int flag) { //если flag = 0, то в файле нет весов ребер, иначе  - есть
	std::ifstream file(filename, std::ios::in);
	file >> N >> M;
	int x, y;
	adj = std::vector<std::unordered_map<int, double>>(N);
	if (flag == 0) {
		while (file >> x >> y) {
			adj[x].insert({ y,1 });
			adj[y].insert({ x,1 });
		}
	}
	for (int i = 0; i < N; i++) {
		adj[i].insert({i,0});
	}
}
std::unordered_map<int, double>& graph::operator[](const int& i) {
	return adj[i];
}
const std::unordered_map<int, double> & graph::operator[](const int& i) const {
	return adj[i];
}
int graph::getVertexCount() const { return N; }
int graph::getEdgeCount() const { return M; }
void graph::printAdjList() const {
	for (int v = 0; v < adj.size(); v++) {
		std::cout << v << ": ";
		for (auto it = adj[v].begin(); it != adj[v].end(); it++) {
			std::cout << "{" << (*it).first << "," << (*it).second << "} ";
		}
		std::cout << std::endl;
	}
}
void graph::printNeighbours(const int& v) const {
	std::cout << v << ": ";
	for (auto it = adj[v].begin(); it != adj[v].end(); it++) {
		std::cout << (*it).first << " ";
	}
	std::cout << std::endl;
}
double graph::getWeightOfLoop(const int& v) const {
	return (*adj[v].find(v)).second;
}

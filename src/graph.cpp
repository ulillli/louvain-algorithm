#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "graph.h"

graph::graph(std::vector<std::vector<int>> v, int n, int m) :N(n), M(m), adj(v) {}
graph::graph(const char* filename) {
		std::ifstream file(filename, std::ios::in);
		file >> N >> M;
		int x, y;
		adj = std::vector<std::vector<int>>(N);
		while (file >> x >> y) {
			adj[x].push_back(y);
			adj[y].push_back(x);
		}
		//M *= 2; //���� ��� ��� ��
}
std::vector<int>& graph::operator[](const int& i) {
		return adj[i];
	}
std::vector<int> graph::operator[](const int& i) const {
		return adj[i];
}
int graph::getVertexCount() const { return N; }
int graph::getEdgeCount() const { return M; }
void graph::printAdjList() const {
	for (int i = 0; i < adj.size(); i++) {
		std::cout << i << ": ";
		for (int j = 0; j < adj[i].size(); j++) {
			std::cout << adj[i][j] << " ";
		}
		std::cout << std::endl;
	}
}


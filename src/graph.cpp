#pragma once
#include <fstream>
#include <iostream>
#include "graph.h"

graph::graph(std::vector<std::unordered_map<int, double>> v, std::vector <double> l , int n, int m) :N(n), M(m), adj(v), loops(l){}
graph::graph(const char* filename, int flag) { //если flag = 0, то в файле нет весов ребер, иначе  - есть
	std::ifstream file(filename, std::ios::in);
	file >> N >> M;
	int x, y;
	adj = std::vector<std::unordered_map<int, double>>(N);
	loops = std::vector<double>(N);
	if (flag == 0) {
		while (file >> x >> y) {
			if (x != y) { //если вдруг случайно в файле затесались петли их пропускаем 
				adj[x].insert({ y,1.0 });
				adj[y].insert({ x,1.0 });
			}
		}
		for (int i = 0; i < N; i++) { //добавляем петли сами с 0-ым весом
			adj[i].insert({ i,0.0 });
			loops[i] = 0.0;
		}
		float m = 0; 
		for (int v = 0; v < adj.size(); v++) {
			for (auto it = adj[v].begin(); it!=adj[v].end(); it++) {
				m += (*it).second;
			}
		}
		M = m/2.0;
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
	return loops[v];
}

#pragma once
#include <fstream>
#include <iostream>
#include <vector>

class graph {
	int N, M;
	std::vector<std::vector<int>> adj;
public:
	graph(std::vector<std::vector<int>> v = std::vector<std::vector<int>>(), int n = 0, int m = 0);
	graph(const char* filename);
	std::vector<int>& operator[](const int& i);
	std::vector<int> operator[](const int& i) const;
	int getVertexCount() const ;
	int getEdgeCount() const ;
	void printAdjList() const ;
};

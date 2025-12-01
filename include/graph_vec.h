#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include<unordered_map>

class graph_vec {
	int N;
	double M;
	std::vector<std::vector<std::pair<int,double>>> adj;
	std::vector<std::vector<int>> exist;
	std::vector<double> loops;
public:
	graph_vec(std::vector<std::vector<std::pair<int, double>>> v = std::vector<std::vector<std::pair<int, double>>>(), std::vector<double> l = std::vector<double>(),  int n = 0, int m = 0);
	graph_vec(const char* filename, int flag=0);
	std::vector<std::pair<int, double>> & operator[](const int& i);
	const std::vector<std::pair<int, double>> & operator[](const int& i) const;
	double getWeightOfLoop(const int& v) const;
	int getVertexCount() const;
	int getEdgeCount() const;
	void printAdjList() const;
	void printNeighbours(const int& v) const;
};
#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include<unordered_map>

class graph {
	int N;
	double M;
	std::vector<std::unordered_map<int,double>> adj;
	std::vector <double> loops;
public:
	graph(std::vector<std::unordered_map<int, double>> v = std::vector<std::unordered_map<int, double>>(), std::vector <double> l = std::vector <double>(),int n = 0, int m = 0);
	graph(const char* filename, int flag=0);
	std::unordered_map<int, double>& operator[](const int& i);
	const std::unordered_map<int, double> & operator[](const int& i) const;
	double getWeightOfLoop(const int& v) const;
	int getVertexCount() const;
	int getEdgeCount() const;
	void printAdjList() const;
	void printNeighbours(const int& v) const;
};
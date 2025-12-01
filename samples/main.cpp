#include "louvain.h"
#include "louvain_vec.h"
#include <string>

int main() {
	//for (int i = 14; i < 15; i++) {
	//	std::string s = "../../data/graph" + std::to_string(i) + ".txt";
	//	graph G(s.data());
	//	std::cout << "Count of nodes: " << G.getVertexCount() << std::endl;
	//	std::cout << "Weight of edges = " << G.getEdgeCount() << std::endl;
	//	louvain L(G);
	//	//L.printResultCommunities();
	//	std::cout << std::endl;
	//}
	for (int i = 13; i < 14; i++) {
		std::string s = "../../data/graph" + std::to_string(i) + ".txt";
		graph_vec G(s.data());
		std::cout << "Count of nodes: " << G.getVertexCount() << std::endl;
		std::cout << "Weight of edges = " << G.getEdgeCount() << std::endl;
		louvain_vec L(G);
		std::cout << std::endl;
	}
	return 0;
}
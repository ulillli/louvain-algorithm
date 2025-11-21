#include "louvain.h"
#include <string>

int main() {
    
	for (int i = 1; i < 13; i++) {
		std::string s = "../../data/graph" + std::to_string(i) + ".txt";
		graph G(s.data());
		std::cout << "Count of nodes: " << G.getVertexCount() << std::endl;
		louvain_new L(G);
		//L.printResultCommunities();
		std::cout << std::endl;
	}
	return 0;
}
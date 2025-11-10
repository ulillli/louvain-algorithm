#include "louvain.h"
#include <string>

int main() {
    
	for (int i = 1; i < 8; i++) {
		std::string s = "../../data/graph" + std::to_string(i) + ".txt";
		graph G(s.data());
		std::cout << "Count of nodes: " << G.getVertexCount() << std::endl;
		louvain_new L(G);
		std::cout << "Count of communities: " << L.getCommunitiesCount() << std::endl;
		//L.printCommunities();
		//L.printPartition();
		std::cout << std::endl;
	}
	return 0;
}
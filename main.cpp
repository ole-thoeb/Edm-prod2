#include "graph.h"
#include "fordFulkerson.h"
#include <vector>
#include <chrono>
#include <iostream>

int main(int argc, char const *argv[]) {
	unsigned long ttStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (argc > 1) {
		Graph g(argv[1], Graph::directed);
		unsigned long tStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::cout << "flow value: " << fordFulkerson(g) << "\n";
		unsigned long tEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::cout << "took " << tEnd -tStart << " ms\n";
	} else {
		std::cout << "no graph (╯°□°）╯︵ ┻━┻\n";
	}
	unsigned long ttEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::cout << "total time " << ttEnd -ttStart << " ms\n";
	return 0;
}

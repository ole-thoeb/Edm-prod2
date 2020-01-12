#include <vector>
#include <iostream>
#include <cmath>
#include <deque>
#include "graph.h"

int U(Graph &g) {
	int sum = 0;
	for (Graph::NodeId id = 0; id < g.num_nodes(); id++) {
		Graph::Node node = g.get_node(id);
		for (auto edge : node.edges()) {
			sum += edge->weight;
		}
	}
	return sum;
}

/**
 * retuns the the path from start to end or an empty vector if no such path exists
**/
std::vector<Graph::Edge*> findPath(Graph &g, Graph::NodeId start, Graph::NodeId end, std::vector<Graph::Edge*> &path, std::vector<bool> &visited, int scaling_value) {
	if (start == end) {
		return path;
	}
	for(Graph::Edge* edge : g.get_node(start).edges()){
		if ((edge->weight >= scaling_value) and not visited[edge->to]){
			visited[edge->to] = true;
			path.push_back(edge);
			std::vector<Graph::Edge*> result = findPath(g, edge->to, end, path, visited, scaling_value);
			if(not result.empty()){
				return result;
			}
			path.pop_back();
		}
	}
	return {};
}

int fordFulkerson(Graph &g) {
	int delta = 2 << (int)std::floor(std::log(U(g)));
	int flowValue = 0;
	while (delta >= 1) {
		std::vector<Graph::Edge*> augmentingPath;
		std::vector<bool> v(g.num_nodes());
		augmentingPath = findPath(g, 0, 1, augmentingPath, v, delta);
		while (not augmentingPath.empty()) {
			//g.print();
			int min = augmentingPath[0]->weight;
			for (size_t i = 1; i < augmentingPath.size(); i++) {
				auto edge = augmentingPath[i];
				if (edge->weight < min) {
					min = edge->weight;
				}
			}
			flowValue += min;
			for (Graph::Edge* edge : augmentingPath) {
				edge->weight -= min;
				edge->revers->weight += min;
			}
			augmentingPath.clear();
			std::fill(v.begin(), v.end(), false);
			augmentingPath = findPath(g, 0, 1, augmentingPath, v, delta);
		}
		delta /= 2;
	}
	return flowValue;
}
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
 * retuns the the path from start to end in reversed order or an empty vector if no such path exists
**/
std::vector<Graph::Edge*>* findPath(Graph &g, Graph::NodeId start, Graph::NodeId end, int limit) {
	std::vector<Graph::NodeId> parent(g.num_nodes());
	std::vector<bool> visited(g.num_nodes());
	std::deque<Graph::Node> queue;
	visited[start] = true;
	queue.push_back(g.get_node(start));
	
	while (queue.size() != 0) {
		//std::cout << "current queue is ";
		//for (Graph::Node e : queue) {
		//	std::cout << e.id() << ", ";
		//}
		//std::cout << "\n";
		Graph::Node u = queue.front();
		queue.pop_front();
		for (Graph::Edge *edge : u.edges()) {
			Graph::Node v = g.get_node(edge->to);
			//std::cout << "looking at edge " << edge->from << " - " << edge->to << " with weight " << edge->weight << "\n";
			if (edge->weight >= limit && not visited[v.id()]) {
				//std::cout << "found " << v.id() << "\n";
				queue.push_back(v);
				visited[v.id()] = true;
				parent[v.id()] = u.id();
			}
		}
	}
	std::vector<Graph::Edge*> *path = new std::vector<Graph::Edge*>();
	if (visited[end]) {
		Graph::NodeId v = end;
		while (v != start) {
			Graph::NodeId u = parent[v];
			path->push_back(g.matrix()[u][v]);
			v = u;
		}
	}
	return path;
}

int fordFulkerson(Graph &g) {
	int delta = 2 << (int)std::floor(std::log(U(g)));
	int flowValue = 0;
	while (delta >= 1) {
		std::vector<Graph::Edge*> *augmentingPath = findPath(g, 0, 1, delta);
		while (not augmentingPath->empty()) {
			//g.print();
			int min = (*augmentingPath)[0]->weight;
			for (size_t i = 1; i < augmentingPath->size(); i++) {
				auto edge = (*augmentingPath)[i];
				if (edge->weight < min) {
					min = edge->weight;
				}
			}
			flowValue += min;
			for (Graph::Edge* edge : *augmentingPath) {
				edge->weight -= min;
				g.getOrCreateEdge(edge->to, edge->from)->weight += min;
			}
			delete augmentingPath;
			augmentingPath = findPath(g, 0, 1, delta);
		}
		//std::cout << "size " << augmentingPath.size() << "\n";
		delete augmentingPath;
		delta /= 2;
	}
	//g.print();
	return flowValue;
}
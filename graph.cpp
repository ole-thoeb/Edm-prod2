// graph.cpp (Implementation of Class Graph)

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include "graph.h"

const Graph::NodeId Graph::invalid_node = -1;
const double Graph::infinite_weight = std::numeric_limits<double>::max();

void Graph::add_nodes(NodeId num_new_nodes) {
    int oldNum = num_nodes();
    int totalNum = oldNum + num_new_nodes;
    for (auto &row : _matrix) {
        row.resize(totalNum);
    }
    for (int i = 0; i < num_new_nodes; i++) {
        _nodes.push_back(Node(oldNum + i));
        _matrix.push_back(std::vector<Graph::Edge*>(totalNum));
    }
    int s = 0;
    for (size_t i = 0; i < 1000000; i++) {
        s = i;
    }
    
}

//Graph::Neighbor::Neighbor(Graph::NodeId n, int w) : _id(n), _edge_weight(w) {}

Graph::Graph(NodeId num, DirType dtype) : dirtype(dtype) {
    add_nodes(num);
}

Graph::~Graph() {
    for (auto &row : _matrix) {
        for (auto edge : row) {
            if (edge != nullptr) {
                delete edge;
            }
        }
    }
}

void Graph::add_edge(NodeId tail, NodeId head, int weight, int index) {
    if (tail >= num_nodes() or tail < 0 or head >= num_nodes() or head < 0) {
        throw std::runtime_error("Edge cannot be added due to undefined endpoint.");
    }
    addSingleEdge(tail, head, weight, index);
    if (dirtype == Graph::undirected) {
        addSingleEdge(head, tail, weight, index);
    }
}

void Graph::addSingleEdge(NodeId from, NodeId to, int weight, int index) {
    getOrCreateEdge(from, to)->addEdge(index, weight);
}

Graph::Edge* Graph::getOrCreateEdge(Graph::NodeId from, Graph::NodeId to) {
    Graph::Edge *edge = _matrix[from][to];
    if (edge == nullptr) {
        edge = new Graph::Edge(from, to);
        _matrix[from][to] = edge;
        get_node(from).edges().push_back(edge);
    }
    return edge;
}

std::vector<std::vector<Graph::Edge*>>& Graph::matrix() {
    return _matrix;
}

Graph::Node::Node(Graph::NodeId id) : _id(id) {}
//Graph::Node::Node() {}

const Graph::NodeId Graph::Node::id() {
    return _id;
}

std::vector<Graph::Edge*> &Graph::Node::edges() {
    return _edges;
}

Graph::NodeId Graph::num_nodes() const
{
    return _nodes.size();
}

Graph::Node &Graph::get_node(NodeId node)
{
    if (node < 0 or node >= static_cast<int>(_nodes.size()))
    {
        throw std::runtime_error("Invalid nodeid in Graph::get_node.");
    }
    return _nodes[node];
}

// Graph::NodeId Graph::Neighbor::id() const
//{
//    return _id;
//}
//
//int Graph::Neighbor::edge_weight() const
//{
//    return _edge_weight;
//}

Graph::Edge::Edge(Graph::NodeId from, Graph::NodeId to) : weight(0), from(from), to(to) {}

void Graph::Edge::addEdge(int edgeId, int weight) {
    this->weight += weight;
    composit.push_back(std::pair(edgeId, weight));
}

void Graph::print()
{
    if (dirtype == Graph::directed)
    {
        std::cout << "Digraph ";
    }
    else
    {
        std::cout << "Undirected graph ";
    }
    std::cout << "with " << num_nodes() << " vertices, numbered 0,...,"
              << num_nodes() - 1 << ".\n";

    for (auto nodeid = 0; nodeid < num_nodes(); ++nodeid)
    {
        std::cout << "The following edges are ";
        if (dirtype == Graph::directed)
        {
            std::cout << "leaving";
        }
        else
        {
            std::cout << "incident to";
        }
        std::cout << " vertex " << nodeid << ":\n";
        for (auto edge : _nodes[nodeid].edges()) {
            std::cout << nodeid << " - " << edge->to
                      << " weight = " << edge->weight << "\n";
        }
    }
}

Graph::Graph(char const *filename, DirType dtype) : dirtype(dtype)
{
    std::ifstream file(filename); // open file
    if (not file)
    {
        throw std::runtime_error("Cannot open file.");
    }

    Graph::NodeId num = 0;
    std::string line;
    std::getline(file, line);   // get first line of file
    std::stringstream ss(line); // convert line to a stringstream
    ss >> num;                  // for which we can use >>
    if (not ss) {
        throw std::runtime_error("Invalid file format.");
    }
    add_nodes(num);
    
    int index = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Graph::NodeId head, tail;
        ss >> tail >> head;
        if (not ss)
        {
            throw std::runtime_error("Invalid file format.");
        }
        int weight = 1;
        ss >> weight;
        if (tail != head) {
            add_edge(tail, head, weight, index);
        } else {
            throw std::runtime_error("Invalid file format: loops not allowed.");
        }
        index++;
    }
}
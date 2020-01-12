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
    for (int i = 0; i < num_new_nodes; i++) {
        _nodes.push_back(Graph::Node(oldNum + i));
    }
}

Graph::Graph(NodeId num, DirType dtype) : dirtype(dtype) {
    add_nodes(num);
}

Graph::~Graph() {
    for (auto &node : _nodes) {
        for (auto edge : node.edges()) {
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
    Graph::Edge *ftt = new Graph::Edge(tail, head,  weight, index);
    _nodes[tail].addEdge(ftt);
    _edges.push_back(ftt);

    Graph::Edge *ttf = new Graph::Edge(head, tail, 0, -index - 1);
    _nodes[head].addEdge(ttf);

    ftt->revers = ttf;
    ttf->revers = ftt;
}

Graph::Node::Node(Graph::NodeId id) : _id(id) {}

const Graph::NodeId Graph::Node::id() {
    return _id;
}

std::vector<Graph::Edge*> &Graph::Node::edges() {
    return _edges;
}

void Graph::Node::addEdge(Graph::Edge *edge) {
    _edges.push_back(edge);
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

std::vector<Graph::Edge*> &Graph::edges() {
    return _edges;
}

Graph::Edge::Edge(Graph::NodeId from, Graph::NodeId to, int weight, int id) : weight(weight), from(from), to(to), id(id), revers(nullptr) {}

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
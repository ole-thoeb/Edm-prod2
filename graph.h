// graph.h (Declaration of Class Graph)
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>

class Graph {
public:
    using NodeId = int;  // vertices are numbered 0,...,num_nodes()-1

    class Edge {
    public:
        int weight; // sum of the weights
        Graph::NodeId from;
        Graph::NodeId to;
        int id;
        Edge *revers;
        Edge(Graph::NodeId from, Graph::NodeId to, int weight, int id);
    };

    class Node {
    public:
        std::vector<Graph::Edge*> & edges();
        const Graph::NodeId id();
        Node(Graph::NodeId id);
        void addEdge(Edge *edge);
        //Node();
    private:
        std::vector<Graph::Edge*> _edges;
        Graph::NodeId _id;
    };

    enum DirType {directed, undirected};  // enum defines a type with possible values
    Graph(NodeId num_nodes, DirType dirtype);
    Graph(char const* filename, DirType dirtype);
    ~Graph();

    void add_nodes(NodeId num_new_nodes);
    void add_edge(NodeId from, NodeId to, int weight, int index);

    NodeId num_nodes() const;
    Node & get_node(NodeId);
    std::vector<Graph::Edge*> &edges();
    void print();

    const DirType dirtype;
    static const NodeId invalid_node;
    static const double infinite_weight;

private:
    std::vector<Node> _nodes;
    std::vector<Edge*> _edges;
};

#endif // GRAPH_H
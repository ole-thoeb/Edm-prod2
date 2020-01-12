// graph.h (Declaration of Class Graph)
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>

class Graph {
public:
    using NodeId = int;  // vertices are numbered 0,...,num_nodes()-1

    //class Neighbor {
    //public:
    //    Neighbor(Graph::NodeId n, int w);
    //    int edge_weight() const;
    //    Graph::NodeId id() const;
    //private:
    //    Graph::NodeId _id;
    //    int _edge_weight;
    //};

    class Edge {
    public:
        int weight; // sum of the weights
        Graph::NodeId from;
        Graph::NodeId to;
        std::vector<std::pair<int, int>> composit; //id to weight
        Edge(Graph::NodeId from, Graph::NodeId to);
        void addEdge(int edgeId, int weight);
    };

    class Node {
    public:
        std::vector<Graph::Edge*> & edges();
        const Graph::NodeId id();
        Node(Graph::NodeId id);
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
    Edge* getOrCreateEdge(NodeId from, NodeId to);
    std::vector<std::vector<Edge*>>& matrix();
    void print();

    const DirType dirtype;
    static const NodeId invalid_node;
    static const double infinite_weight;

private:
    void addSingleEdge(NodeId from, NodeId to, int weight, int index);
    std::vector<Node> _nodes;
    std::vector<std::vector<Edge*>> _matrix;
};

#endif // GRAPH_H
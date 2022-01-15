//
// Created by Ryan.Zurrin001 on 1/14/2022.
//

#ifndef FLOWNETWORK_FLOWNETWORK_H
#define FLOWNETWORK_FLOWNETWORK_H
#include "FlowEdge.h"
#include "Bag.h"



class FlowNetwork {
    int _V;
    int _E;
    Bag<FlowEdge*> *_adj;
public:
    FlowNetwork(int V);
    FlowNetwork(int V, int E);
    FlowNetwork(istream &in);
    ~FlowNetwork();
    void validateVertex(int v) const;
    void addEdge(FlowEdge* e);
    void removeEdge(FlowEdge *pEdge);
    int V() const;
    int E() const;
    // returns the edges incident to v, including both edges pointing to and from v
    Bag<FlowEdge*>::Iterator adj(int v);
    // a method that returns an iterator of the edges not including self loops
    Bag<FlowEdge*>::Iterator edges();
    string toString() const;
    // overloads the << operator
    friend ostream &operator<<(ostream &os, const FlowNetwork &G);

    // inner class for iterating over the edges
    class FlowNetworkIterator : public Bag<FlowEdge>::Iterator {
    public:
        FlowNetworkIterator(Bag<FlowEdge>::Iterator *it) : Bag<FlowEdge>::Iterator(
                reinterpret_cast<Node<struct FlowEdge> *>(it)) {}
        FlowNetworkIterator(const FlowNetworkIterator &it) : Bag<FlowEdge>::Iterator(it) {}
        FlowNetworkIterator &operator=(const FlowNetworkIterator &it) {
            Bag<FlowEdge>::Iterator::operator=(it);
            return *this;
        }
        FlowNetworkIterator &operator++() {
            Bag<FlowEdge>::Iterator::operator++();
            return *this;
        }
        FlowNetworkIterator operator++(int) {
            FlowNetworkIterator it = *this;
            operator++();
            return it;
        }
    };

};


FlowNetwork::FlowNetwork(int V) {
    if (V < 0) throw "Number of vertices must be nonnegative";
    _V = V;
    _E = 0;
    _adj = new Bag<FlowEdge*>[V];
    for (int v = 0; v < V; v++) {
        _adj[v] = Bag<FlowEdge*>();
    }
}

FlowNetwork::FlowNetwork(int V, int E) {
    if (V < 0) throw "Number of vertices must be nonnegative";
    if (E < 0) throw "Number of edges must be nonnegative";
    _V = V;
    _E = 0;
    _adj = new Bag<FlowEdge*>[V];
    for (int v = 0; v < V; v++) {
        _adj[v] = Bag<FlowEdge*>();
    }
    // initialize a random flow network with V vertices and E edges. The
    // capacities are between 0 and 99 and the flow values are 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> vert(0, V - 1);
    std::uniform_int_distribution<> dis(0, 99); // for capacities
    for (int i = 0; i < E; i++) {
        // randomly generate the vertices from 0 to V-1
        int v = vert(gen);
        int w = vert(gen);
        int c = dis(gen);
        addEdge(new FlowEdge(v, w, c));
    }
}

FlowNetwork::FlowNetwork(istream &in) {
    in >> _V;
    if (_V < 0) throw "Number of vertices must be nonnegative";
    in >> _E;
    if (_E < 0) throw "Number of edges must be nonnegative";
    _adj = new Bag<FlowEdge*>[V()];
    for (int v = 0; v < V(); v++) {
        _adj[v] = Bag<FlowEdge*>();
    }
    for (int i = 0; i < E(); i++) {
        int v, w;
        double c;
        in >> v >> w >> c;
        if (v < 0 || v >= V()) throw "vertex " + to_string(v) + " is not between 0 and " + to_string(V() - 1);
        if (w < 0 || w >= V()) throw "vertex " + to_string(w) + " is not between 0 and " + to_string(V() - 1);
        addEdge(new FlowEdge(v, w, c));
    }
}

FlowNetwork::~FlowNetwork() {
    unordered_set<FlowEdge*> edges;
    // before deleting the edge add to a set to avoid duplicate deletion
    for (int v = 0; v < V(); v++) {
        for (int i = 0; i < _adj[v].size(); i++) {
            edges.insert(_adj[v].get(i));
        }
    }
    for (auto e : edges) {
        delete e;
    }
    delete[] _adj;
}

void FlowNetwork::validateVertex(int v) const {
    if (v < 0 || v >= V()) throw "vertex " + to_string(v) +
    " is not between 0 and " + to_string(V() - 1);
}
void FlowNetwork::addEdge(FlowEdge* e) {
    int v = e->from();
    int w = e->to();
    validateVertex(v);
    validateVertex(w);
    _adj[v].add(e);
    _adj[w].add(e);
    _E++;
}

int FlowNetwork::V() const {
    return _V;
}

int FlowNetwork::E() const {
    return _E;
}

Bag<FlowEdge*>::Iterator FlowNetwork::adj(int v)  {
    validateVertex(v);
    return _adj[v].begin();
}

Bag<FlowEdge*>::Iterator FlowNetwork::edges()  {
    Bag<FlowEdge*> bag;
    for (int v = 0; v < V(); v++) {
        for (FlowEdge* e : _adj[v]) {
            if (e->from() != v) bag.add(e);
        }
    }
    return bag.begin();
}

string FlowNetwork::toString() const {
    stringstream ss;
    ss << V() << " " << E() << endl;
    for (int v = 0; v < V(); v++) {
        ss << "  " << v << ": ";
        for (Bag<FlowEdge*>::Iterator it = _adj[v].begin(); it != _adj[v].end(); ++it) {
            ss << *(*it) << ", ";
        }
        ss << endl;
    }
    return ss.str();
}

ostream &operator<<(ostream &os, const FlowNetwork &G) {
    os << G.toString();
    return os;
}

void FlowNetwork::removeEdge(FlowEdge *pEdge) {
    // remove all occurrences of pEdge from _adj
    int v = pEdge->from();
    int w = pEdge->to();
    _adj[v].remove(pEdge);
    _adj[w].remove(pEdge);
    delete pEdge;
    _E--;
}

#endif //FLOWNETWORK_FLOWNETWORK_H

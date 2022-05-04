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
    int _source;
    int _sink;
public:
    Bag<FlowEdge*> *_adj;
    FlowNetwork(int V);
    FlowNetwork(int V, int E, int source, int sink,
                bool selfLoops = false, bool parallelEdges = false );
    FlowNetwork(istream &in, int source, int sink, bool selfLoops = false, bool parallelEdges = false);
    ~FlowNetwork();
    void validateVertex(int v) const;
    void addEdge(FlowEdge* e);
    void addEdge(int v, int w, double capacity);
    void removeEdge(FlowEdge *pEdge);
    bool containsEdge(FlowEdge *pEdge);
    int V() const;
    int E() const;
    int source() const { return _source; }
    int sink() const { return _sink; }
    void setSource(int source) { _source = source; }
    void setSink(int sink) { _sink = sink; }
    Bag<FlowEdge*>::Iterator adj(int v);
    Bag<FlowEdge*>::Iterator edges();

    string toString() const;
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
    _source = 0;
    _sink = V - 1;
    _adj = new Bag<FlowEdge*>[V];
    for (int v = 0; v < V; v++) {
        _adj[v] = Bag<FlowEdge*>();
    }
}

FlowNetwork::FlowNetwork(int V, int E, int source, int sink,
                         bool selfLoops, bool parallelEdges) {
    if (V < 0) throw "Number of vertices must be nonnegative";
    if (E < 0) throw "Number of edges must be nonnegative";
    if (source < 0 || source >= V) throw "Invalid source";
    if (sink < 0 || sink >= V) throw "Invalid sink";
    _V = V;
    _E = 0;
    _source = source;
    _sink = sink;
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
        while (v == _sink) v = vert(gen); // no output edges from the sink
        while (w == _source) w = vert(gen); // no input edges to the source
        if (!selfLoops && v == w) { // no self loops
            i--; continue;
        }
        FlowEdge *e = new FlowEdge(v, w, c);
        if (!parallelEdges && containsEdge(e)) { // no parallel edges
            delete e;
            i--; continue;
        } else {
            addEdge(e);
        }
    }
}

FlowNetwork::FlowNetwork(istream &in, int source, int sink, bool selfLoops, bool parallelEdges) {
    this->setSink(sink);
    this->setSource(source);
    int edges = 0;
    this->_E = 0;
    in >> _V;
    if (_V < 0) throw "Number of vertices must be nonnegative";
    in >> edges;
    if (_E < 0) throw "Number of edges must be nonnegative";
    _adj = new Bag<FlowEdge*>[V()];
    for (int v = 0; v < V(); v++) {
        _adj[v] = Bag<FlowEdge*>();
    }
    for (int i = 0; i < edges; i++) {
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
        if (e != nullptr) delete e;
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
            if ((*it)->to() != v) ss << *(*it) << ", ";
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

bool FlowNetwork::containsEdge(FlowEdge *pEdge) {
    // remove all occurrences of pEdge from _adj
    int v = pEdge->from();
    int w = pEdge->to();
    for (Bag<FlowEdge*>::Iterator it = _adj[v].begin(); it != _adj[v].end(); it++) {
        // check that the to and from vertices are the same
        if ((*it)->to() == w && (*it)->from() == v) {
            return true;
        }
    }
    return false;
}

void FlowNetwork::addEdge(int v, int w, double capacity) {
    if (v < 0 || v >= V()) throw "vertex " + to_string(v) + " is not between 0 and " + to_string(V() - 1);
    if (w < 0 || w >= V()) throw "vertex " + to_string(w) + " is not between 0 and " + to_string(V() - 1);
    FlowEdge *e = new FlowEdge(v, w, capacity);
    if (containsEdge(e)) {
        delete e;
        return;
    } else {
        addEdge(e);
    }
}

#endif //FLOWNETWORK_FLOWNETWORK_H

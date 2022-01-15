//
// Created by Ryan.Zurrin001 on 1/15/2022.
//

#ifndef FLOWNETWORK_FLOW_NETWORK_H
#define FLOWNETWORK_FLOW_NETWORK_H
#include "FlowEdge.h"

class Flow_Network {
public:
    Flow_Network(int V);
    Flow_Network(int V, int E);
    Flow_Network(const Flow_Network& other);
    Flow_Network& operator=(const Flow_Network& other);
    ~Flow_Network();

    int V() const;
    int E() const;
    int flow() const;
    void addEdge(FlowEdge* e);
    void addEdge(int v, int w, double capacity);
    void addEdge(int v, int w, double capacity, double cost);
    bool validEdge(int v, int w) const;
    // return vector iterator to the edges
    vector<FlowEdge*>::iterator adj(int v);
    vector<FlowEdge*>::iterator edges() const;
    // get the edge
    FlowEdge* edge(int v, int w) const;
    double edgeCost(int v, int w) const;
    // get val at the edge
    // overload the << operator to print the graph
    friend ostream& operator<<(ostream& os, const Flow_Network& g);
    // overload the subscript operator to return the edge
    FlowEdge& operator[](int v);
    // overload the () operator to return the edge
    FlowEdge& operator()(int v);
    FlowEdge& operator()(int v, int w);

    FlowEdge *edge(int i);
    vector<FlowEdge*>* _adj;


    double get_capacity(int &i);

    void set_capacity(int &i, double d);

private:
    int _V;
    int _E;


};

Flow_Network::Flow_Network(int V) {
    _V = V;
    _E = 0;
    _adj = new vector<FlowEdge*>[_V];
}

Flow_Network::Flow_Network(int V, int E) {
    if (V < 0) {
        throw std::invalid_argument("Number of vertices must be nonnegative");
    }
    if (E < 0) {
        throw std::invalid_argument("Number of edges must be nonnegative");
    }
    _V = V;
    _E = 0;
    _adj = new vector<FlowEdge*>[_V];
    // initialize a random flow network with V vertices and E edges. The capacities
    // are integer values between 0 and 99 and the flow values are all 0.
    srand(time(NULL));
    for (int i = 0; i < E; i++) {
        int v = rand() % V;
        int w = rand() % V;
        int c = rand() % 100;
        while (v == w) {
            w = rand() % V;
        }
        // check to see if the edge already exists
        if (validEdge(v, w)) {
            i--;
            continue;
        }
        addEdge(v, w, c);
    }
}

Flow_Network::Flow_Network(const Flow_Network &other) {
    _V = other._V;
    _E = other._E;
    _adj = new vector<FlowEdge*>[_V];
    for (int v = 0; v < _V; v++) {
        for (FlowEdge* e : other._adj[v]) {
            _adj[v].push_back(new FlowEdge(*e));
        }
    }
}

Flow_Network &Flow_Network::operator=(const Flow_Network &other) {
    if (this != &other) {
        _V = other._V;
        _E = other._E;
        _adj = new vector<FlowEdge*>[_V];
        for (int v = 0; v < _V; v++) {
            for (FlowEdge* e : other._adj[v]) {
                _adj[v].push_back(new FlowEdge(*e));
            }
        }
    }
    return *this;
}

Flow_Network::~Flow_Network() {
    for (int v = 0; v < _V; v++) {
        for (FlowEdge* e : _adj[v]) {
            delete e;
        }
    }
    delete[] _adj;
}

int Flow_Network::V() const {
    return _V;
}

int Flow_Network::flow() const {
    int flow = 0;
    for (int v = 0; v < _V; v++) {
        for (FlowEdge* e : _adj[v]) {
            flow += e->flow();
        }
    }
    return flow;
}

int Flow_Network::E() const {
    return _E;
}

void Flow_Network::addEdge(FlowEdge *e) {
    int v = e->from();
    int w = e->to();
    if (v < 0 || v >= _V) {
        cout << "vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(_V - 1);
        return;
    }
    if (w < 0 || w >= _V) {
        cout << "vertex " + std::to_string(w) + " is not between 0 and " + std::to_string(_V - 1) << endl;
        return;
    }
    if (v == w) {
        cout << "vertex " + std::to_string(v) + " is equal to vertex " + std::to_string(w) + "\n";
        return;
    }
    if (validEdge(v, w)) {
        cout << "edge (" + std::to_string(v) + ", " + std::to_string(w) + ") is a duplicate" << endl;
        return;
    }
    _adj[v].push_back(e);
    _E++;
}

void Flow_Network::addEdge(int v, int w, double capacity) {
    if (v < 0 || v >= _V) {
        cout << "vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(_V - 1);
        return;
    }
    if (w < 0 || w >= _V) {
        cout << "vertex " + std::to_string(w) + " is not between 0 and " + std::to_string(_V - 1) << endl;
        return;
    }
    if (v == w) {
        cout << "vertex " + std::to_string(v) + " is equal to vertex " + std::to_string(w) + "\n";
        return;
    }
    if (validEdge(v, w)) {
        cout << "edge (" + std::to_string(v) + ", " + std::to_string(w) + ") is a duplicate" << endl;
        return;
    }
    _adj[v].push_back(new FlowEdge(v, w, capacity));
    _E++;
}

void Flow_Network::addEdge(int v, int w, double capacity, double cost) {
    if (v < 0 || v >= _V) {
        cout << "vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(_V - 1);
        return;
    }
    if (w < 0 || w >= _V) {
        cout << "vertex " + std::to_string(w) + " is not between 0 and " + std::to_string(_V - 1) << endl;
        return;
    }
    if (v == w) {
        cout << "vertex " + std::to_string(v) + " is equal to vertex " + std::to_string(w) + "\n";
        return;
    }
    if (validEdge(v, w)) {
        cout << "edge (" + std::to_string(v) + ", " + std::to_string(w) + ") is a duplicate" << endl;
        return;
    }
    _adj[v].push_back(new FlowEdge(v, w, capacity, cost));
    _E++;
}

vector<FlowEdge *>::iterator Flow_Network::adj(int v) {
    if (v < 0 || v >= _V) {
        throw std::invalid_argument("vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(_V - 1));
    }
    return _adj[v].begin();
}

bool Flow_Network::validEdge(int v, int w) const {
    for (FlowEdge* e : _adj[v]) {
        if (e->to() == w) {
            return true;
        }
    }
    return false;
}

vector<FlowEdge *>::iterator Flow_Network::edges() const {
    vector<FlowEdge *> edges;
    for (int v = 0; v < _V; v++) {
        for (FlowEdge* e : _adj[v]) {
            edges.push_back(e);
        }
    }
    return edges.begin();
}

ostream &operator<<(ostream &os, const Flow_Network &g) {
    os << "Flow_Network(" << g.V() << " vertices, " << g.E() << " edges)" << endl;
    for (int v = 0; v < g.V(); v++) {
        os << "  " << v << ": ";
        for (FlowEdge* e : g._adj[v]) {
            os << *e << "  ";
        }
        os << endl;
    }
    return os;
}

FlowEdge &Flow_Network::operator[](int v) {
    if (v < 0 || v >= _V) {
        throw std::invalid_argument("vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(_V - 1));
    }
    return *_adj[v][0];
}

FlowEdge &Flow_Network::operator()(int v) {
    if (v < 0 || v >= _V) {
        throw std::invalid_argument("vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(_V - 1));
    }
    return *_adj[v][0];
}

FlowEdge &Flow_Network::operator()(int v, int w) {
    if (v < 0 || v >= _V) {
        throw std::invalid_argument("vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(_V - 1));
    }
    if (w < 0 || w >= _V) {
        throw std::invalid_argument("vertex " + std::to_string(w) + " is not between 0 and " + std::to_string(_V - 1));
    }
    for (FlowEdge* e : _adj[v]) {
        if (e->to() == w) {
            return *e;
        }
    }
    throw std::invalid_argument("edge (" + std::to_string(v) + ", " + std::to_string(w) + ") is not in the graph");
}

FlowEdge *Flow_Network::edge(int v, int w) const {
    if (v < 0 || v >= _V) {
        throw std::invalid_argument("vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(_V - 1));
    }
    if (w < 0 || w >= _V) {
        throw std::invalid_argument("vertex " + std::to_string(w) + " is not between 0 and " + std::to_string(_V - 1));
    }
    for (FlowEdge* e : _adj[v]) {
        if (e->to() == w) {
            return e;
        }
    }
    return nullptr;
}


FlowEdge *Flow_Network::edge(int i) {
    if (i < 0 || i >= _E) {
        throw std::invalid_argument("edge " + std::to_string(i) + " is not between 0 and " + std::to_string(_E - 1));
    }
    int count = 0;
    for (int v = 0; v < _V; v++) {
        for (FlowEdge* e : _adj[v]) {
            if (count == i) {
                return e;
            }
            count++;
        }
    }
    throw std::invalid_argument("edge " + std::to_string(i) + " is not in the graph");
}

double Flow_Network::edgeCost(int v, int w) const {
    if (v < 0 || v >= _V) {
        throw std::invalid_argument("vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(_V - 1));
    }
    if (w < 0 || w >= _V) {
        throw std::invalid_argument("vertex " + std::to_string(w) + " is not between 0 and " + std::to_string(_V - 1));
    }
    for (FlowEdge* e : _adj[v]) {
        if (e->to() == w) {
            return e->capacity() - e->flow();
        }
    }
    throw std::invalid_argument("edge (" + std::to_string(v) + ", " + std::to_string(w) + ") is not in the graph");
}


double  Flow_Network::get_capacity(int &i) {
    if (i < 0 || i >= _E) {
        throw std::invalid_argument("edge " + std::to_string(i) + " is not between 0 and " + std::to_string(_E - 1));
    }
    int count = 0;
    for (int v = 0; v < _V; v++) {
        for (FlowEdge* e : _adj[v]) {
            if (count == i) {
                return e->residualCapacityTo(v);
            }
            count++;
        }
    }
    throw std::invalid_argument("edge " + std::to_string(i) + " is not in the graph");
}

void Flow_Network::set_capacity(int &i, double d) {
    // adds resodiial flow to the edge
    if (i < 0 || i >= _E) {
        throw std::invalid_argument("edge " + std::to_string(i) + " is not between 0 and " + std::to_string(_E - 1));
    }
    int count = 0;
    for (int v = 0; v < _V; v++) {
        for (FlowEdge* e : _adj[v]) {
            if (count == i) {
                e->addResidualFlowTo(v, d);
                return;
            }
            count++;
        }
    }
    throw std::invalid_argument("edge " + std::to_string(i) + " is not in the graph");
}


#endif //FLOWNETWORK_FLOW_NETWORK_H

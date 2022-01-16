//
// Created by Ryan.Zurrin001 on 1/15/2022.
//

#ifndef FLOWNETWORK_FORDFULKERSON_H
#define FLOWNETWORK_FORDFULKERSON_H
#include "FlowNetwork.h"

class FordFulkerson {
    int  _V; // number of vertices
    int _source; // source vertex
    int _sink; // sink vertex
    int **_adjMat; // adjacency matrix
    // marked[v] = true iff s->v path in residual graph
    vector<bool> marked;
    // edgeTo[v] = last edge on shortest s->v path
    vector<FlowEdge*> edgeTo;
    double _value;
    FlowNetwork* _network;
    bool bfs(int **adjM, int *parent);
    void dfs(int **adjM, int s, bool* visited);
    vector<pair<int,int>> minCut(int **adjM, int s, int t, bool print = false);
    void flowNetworkToAdjMat(FlowNetwork& G);

public:
    FordFulkerson(FlowNetwork &G);
    double value();
    bool inCut(int v);
    void validate(int v);
    bool hasAugmentingPath(FlowNetwork& G, int s, int t);
    double excess(FlowNetwork& G, int v);
    bool isFeasible(FlowNetwork& G, int s, int t);
    bool check(FlowNetwork& G, int s, int t);
    void minCut();

    string toString();
    // overload << operator
    friend ostream& operator<<(ostream& os, FordFulkerson& ff);
    ~FordFulkerson() {
        _network = nullptr;
        delete _network;
        // loop through the adjacency matrix and delete the pointers
        for (int i = 0; i < _V; i++) {
            delete[] _adjMat[i];
        }
        delete[] _adjMat;
    }
};

FordFulkerson::FordFulkerson(FlowNetwork &G) {
    _V = G.V();
    int s = G.source();
    int t = G.sink();
    validate(s);
    validate(t);
    if (s == t) throw runtime_error("s == t");
    _network = &G;
    _source = s;
    _sink = t;
    _value = 0.0;
    if (!isFeasible(G, s, t)) throw runtime_error("Initial flow is infeasible");
    edgeTo = vector<FlowEdge*>(_V);
    marked = vector<bool>(_V);
    for (int v = 0; v < _V; v++) {
        edgeTo[v] = nullptr;
        marked[v] = false;
    }
    _value= excess(G, t);
    // compute maximum flow and minimum cut
    while (hasAugmentingPath(G, s, t)) {
        double bottle = std::numeric_limits<double>::max();
        for (int v = t; v != s; v = edgeTo[v]->other(v)) {
            bottle = min(bottle, edgeTo[v]->residualCapacityTo(v));
        }
        // augment flow
        for (int v = t; v != s; v = edgeTo[v]->other(v)) {
            edgeTo[v]->addResidualFlowTo(v, bottle);
        }
        _value += bottle;
    }
    // allocate _adjMat with size _V * _E
    _adjMat = new int*[_V];
    for (int i = 0; i < _V; i++) {
        _adjMat[i] = new int[_V];
        for (int j = 0; j < _V; j++) {
            _adjMat[i][j] = 0;
        }
    }
    flowNetworkToAdjMat(*_network);

    // check optimality conditions
    if (!isFeasible(G, s, t)) {
        cout << "*** Flow is infeasible ***" << endl;
    } else if (!check(G, s, t)) {
        cout << "*** check() detects a problem ***" << endl;
    } else {
        cout << "*** Flow is optimal ***" << endl;
    }

}

double FordFulkerson::value() {
    return _value;
}

bool FordFulkerson::inCut(int v) {
    validate(v);
    return marked[v];
}

void FordFulkerson::validate(int v) {
    if (v < 0 || v >= _V)
        throw "vertex " + to_string(v) + " is not between 0 and " +
        to_string(_V - 1);
}
bool FordFulkerson::hasAugmentingPath(FlowNetwork &G, int s, int t) {
    edgeTo = vector<FlowEdge*>(_V);
    marked = vector<bool>(_V);
    for (int v = 0; v < _V; v++) {
        edgeTo[v] = nullptr;
        marked[v] = false;
    }
    queue<int> q;
    q.push(s);
    marked[s] = true;
    while (!q.empty() && !marked[t]) {
        int v = q.front();
        q.pop();
        for (Bag<FlowEdge*>::Iterator e = G._adj[v].begin(); e !=
        G._adj[v].end(); ++e) {
            int w = (*e)->other(v);
            if ((*e)->residualCapacityTo(w) > 0) {
                if (!marked[w]) {
                    this->edgeTo[w] = *e;
                    this->marked[w] = true;
                    q.push(w);
                }
            }
        }
    }
    return marked[t];
}

double FordFulkerson::excess(FlowNetwork &G, int v) {
    double excess = 0.0;
    for (Bag<FlowEdge*>::Iterator e = G._adj[v].begin(); e != G._adj[v].end(); ++e) {
        if ((*e)->from() == v) {
            excess -= (*e)->flow();
        } else {
            excess += (*e)->flow();
        }
    }
    return excess;
}

bool FordFulkerson::isFeasible(FlowNetwork &G, int s, int t) {
    for (int v = 0; v < G.V(); v++) {
        for (Bag<FlowEdge*>::Iterator e = G._adj[v].begin(); e != G._adj[v].end(); ++e) {
            if ((*e)->flow() < -FLOATING_POINT_EPSILON ||
                    (*e)->flow() > (*e)->capacity() + FLOATING_POINT_EPSILON) {
                std::cout << "Edge does not satisfy capacity constraints: " <<
                *e << std::endl;
                return false;
            }
        }
    }
    if (abs(_value + excess(G, s)) > FLOATING_POINT_EPSILON) {
        std::cout << "Excess at source: " << s << " = " << excess(G, s) <<
        ", Max Flow = " << _value << std::endl;
        return false;
    }
    if (abs(_value - excess(G, t)) > FLOATING_POINT_EPSILON) {
        std::cout << "Excess at sink: " << t << " = " << excess(G, t) <<
        ", Max Flow = " << _value << std::endl;
        return false;
    }
    for (int v = 0; v < G.V(); v++) {
        if (v == s || v == t) continue;
        else if (abs(excess(G, v)) > FLOATING_POINT_EPSILON) {
            std::cout << "Net flow out of " << v <<
            " doesn't equal zero" << std::endl;
            return false;
        }
    }
    return true;
}

bool FordFulkerson::check(FlowNetwork &G, int s, int t) {
    if (!isFeasible(G, s, t)) {
        std::cout << "Flow is infeasible" << std::endl; return false;
    }
    if (!inCut(s)) {
        std::cout << "source " << s << " is not on source side of min cut" <<
        std::endl; return false;
    }
    if (inCut(t)) {
        std::cout << "sink " << t << " is on source side of min cut" <<
        std::endl; return false;
    }
    double mincutValue = 0.0;
    for (int v = 0; v < G.V(); v++) {
        for (Bag<FlowEdge*>::Iterator e = G._adj[v].begin(); e != G._adj[v].end(); ++e) {
            if ((v == (*e)->from() && inCut((*e)->from()) && !inCut((*e)->to()))) {
                mincutValue += (*e)->capacity();
            }
        }
    }
    if (abs(mincutValue - _value) > FLOATING_POINT_EPSILON) {
        std::cout << "Max flow value = " << _value << ", min cut value = "
        << mincutValue << std::endl; return false;
    }
    return true;
}

string FordFulkerson::toString() {
    string s = ""; int lnbk = 0;
    s += "Max flow from  " + to_string(_source) + " to " + to_string(_sink) + "\n";
    for (int v = 0; v < _V; v++) {
        for (Bag<FlowEdge*>::Iterator it = _network->_adj[v].begin(); it !=
        _network->_adj[v].end(); ++it) {
            if ((*it)->from() == v&& (*it)->flow() > 0) {
                s+= " (" + (*it)->toString() + ") ";
                lnbk++;
                if (lnbk == 8) {
                    s += "\n";
                    lnbk = 0;
                }
            }
        }
    }
    // minCut prints the edges of the min cut
    vector<pair<int, int>> mc =  minCut(_adjMat, _source, _sink);
    s += "\n\nMin s-t cut:\n";
    for (int i = 0; i < mc.size(); i++) {
        s += "(" +to_string(mc[i].first) + " - " + to_string(mc[i].second) + ")";
        if (i != mc.size() - 1) {
            s += ", ";
            if (i % 8 == 7) {
                s += "\n";
            }
        }
    }
    s += "\n\nMax flow value = " + to_string(_value) + "\n";
    return s;
}

ostream &operator<<(ostream &os, FordFulkerson &ff) {
    // set precision to 2 decimal places
    os << fixed << setprecision(2) << ff.toString();
    return os;
}

bool FordFulkerson::bfs(int** adj,  int *parent) {
    // creates a vistited array and sets all to false
    bool visited[_V];
    for (int i = 0; i < _V; i++)  visited[i] = false;
    // creates a queue and adds the source to it
    queue<int> q;
    q.push(_source);
    visited[_source] = true;
    parent[_source] = -1;
    // while the queue is not empty
    while (!q.empty()) {
        // get the first element in the queue
        int u = q.front();
        q.pop();
        // for each edge in the adjacency list of v
        for (int v = 0; v < _V; v++) {
            if (adj[u][v] > 0 && !visited[v]) {
                // add the vertex to the queue
                q.push(v);
                // set the parent of the vertex to v
                parent[v] = u;
                // set the visited array to true
                visited[v] = true;
            }
        }
    }
    return visited[_sink];
}

void FordFulkerson::dfs(int **adjM, int s, bool *visited) {
    visited[s] = true;
    for (int v = 0; v < _V; v++) {
        if (adjM[s][v] > 0 && !visited[v]) {
            dfs(adjM, v, visited);
        }
    }
}
// prints the minimum s-t cut
vector<pair<int,int>> FordFulkerson::minCut(int **adjM, int s, int t, bool print) {
    int u, v;
    vector <pair<int, int>> cut;
    // create a reisdual graph from the adjacency matrix
    int **residual = new int*[_V];
    for (u = 0; u < _V; u++) {
        residual[u] = new int[_V];
        for (v = 0; v < _V; v++) {
            residual[u][v] = adjM[u][v];
        }
    }
    int *parent = new int[_V];
    // while the source is not in the cut
    while (bfs(residual, parent)) {
        int path_flow = std::numeric_limits<int>::max();
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, residual[u][v]);
        }

        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            residual[u][v] -= path_flow;
            residual[v][u] += path_flow;
        }
    }
    bool *visited = new bool[_V];
    for (int i = 0; i < _V; i++) visited[i] = false;
    dfs(residual, s, visited);
    int linebreak = 0;
    vector<pair<int, int>> cutEdges;
    if (print) cout << "Min Cut:\n";
    for (int i = 0; i < _V; i++) {
        for(int j = 0; j < _V; j++) {
            if (visited[i] && !visited[j] && adjM[i][j]) {
                if (print) cout << "(" << i << ", " << j << ") ";
                cutEdges.push_back(make_pair(i, j));
                linebreak++;
            }
            if (print) if (linebreak % 5 == 0) cout << "\n";
        }
    }
    // delete all the arrays
    for (int i = 0; i < _V; i++) {
        delete[] residual[i];
    }
    delete[] residual;
    delete[] visited;
    delete[] parent;
    return cutEdges;
}

void FordFulkerson::minCut() {
    minCut(_adjMat, _source, _sink);
}

void FordFulkerson::flowNetworkToAdjMat(FlowNetwork &G) {
    // allocate memeory for the adjacency matrix

    // populate the adjacency matrix with the capacities of the edges
    for (int v = 0; v < G.V(); v++) {
        for (Bag<FlowEdge*>::Iterator it = G._adj[v].begin(); it != G._adj[v].end(); ++it) {
            if ((*it)->from() == v) {
                _adjMat[v][(*it)->to()] = (*it)->capacity();
            }
            else {
                _adjMat[v][(*it)->to()] = 0;
            }
        }
    }
}


#endif //FLOWNETWORK_FORDFULKERSON_H

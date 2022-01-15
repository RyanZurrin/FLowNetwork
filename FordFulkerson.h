//
// Created by Ryan.Zurrin001 on 1/15/2022.
//

#ifndef FLOWNETWORK_FORDFULKERSON_H
#define FLOWNETWORK_FORDFULKERSON_H
#include "FlowNetwork.h"

class FordFulkerson {
    int  _V; // number of vertices
    // marked[v] = true iff s->v path in residual graph
    vector<bool> marked;
    // edgeTo[v] = last edge on shortest s->v path
    vector<FlowEdge*> edgeTo;
    double _value;

public:
    FordFulkerson(FlowNetwork &G, int s, int t);
    double value();
    bool inCut(int v);
    void validate(int v);
    bool hasAugmentingPath(FlowNetwork& G, int s, int t);
    double excess(FlowNetwork& G, int v);
    bool isFeasible(FlowNetwork& G, int s, int t);
    bool check(FlowNetwork& G, int s, int t);

};

FordFulkerson::FordFulkerson(FlowNetwork &G, int s, int t) {

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
        throw "vertex " + to_string(v) + " is not between 0 and " + to_string(_V - 1);
}
bool FordFulkerson::hasAugmentingPath(FlowNetwork &G, int s, int t) {
    return false;
}

double FordFulkerson::excess(FlowNetwork &G, int v) {
    double excess = 0.0;
    for (auto* e : G.adj(v)) {
        if (e->from() == v) excess -= e->flow();
        else excess += e->flow();
    }
}

bool FordFulkerson::isFeasible(FlowNetwork &G, int s, int t) {
    return false;
}

bool FordFulkerson::check(FlowNetwork &G, int s, int t) {
    return false;
}


#endif //FLOWNETWORK_FORDFULKERSON_H

//
// Created by Ryan.Zurrin001 on 1/14/2022.
//

#ifndef FLOWNETWORK_FLOWEDGE_H
#define FLOWNETWORK_FLOWEDGE_H
#include <bits/stdc++.h>
using namespace std;
#define FLOATING_POINT_EPSILON 1e-10


class FlowEdge {
public:
    FlowEdge(int v, int w, double capacity);
    FlowEdge(int v, int w, double capacity, double flow);
    FlowEdge(FlowEdge& e);
    FlowEdge(FlowEdge&& e);
    FlowEdge& operator=(FlowEdge& e);
    FlowEdge& operator=(FlowEdge&& e);
    int from() const;
    int to() const;
    double capacity() const;
    double flow() const;
    int other(int vertex) const;
    double residualCapacityTo(int vertex) const;
    void addResidualFlowTo(int v, double delta);
    string toString() const;
    friend ostream& operator<<(ostream& os, const FlowEdge& e);
private:
    int _v;
    int _w;
    double _capacity;
    double _flow;
};

FlowEdge::FlowEdge(int v, int w, double capacity) {
    if (v < 0 || w < 0) {
        throw invalid_argument("vertex index must be nonnegative");
    }
    if (capacity < 0.0) {
        throw invalid_argument("capacity must be nonnegative");
    }
    _v = v;
    _w = w;
    _capacity = capacity;
    _flow = 0.0;
}

FlowEdge::FlowEdge(int v, int w, double capacity, double flow) {
    if (v < 0 || w < 0) {
        throw invalid_argument("vertex index must be nonnegative");
    }
    if (capacity < 0.0) {
        throw invalid_argument("capacity must be nonnegative");
    }
    if (flow < 0.0 || flow > capacity) {
        throw invalid_argument("flow must be nonnegative and less than or equal to capacity");
    }
    _v = v;
    _w = w;
    _capacity = capacity;
    _flow = flow;
}

FlowEdge::FlowEdge(FlowEdge &e) {
    _v = e._v;
    _w = e._w;
    _capacity = e._capacity;
    _flow = e._flow;
}

FlowEdge::FlowEdge(FlowEdge &&e) {
    _v = e._v;
    _w = e._w;
    _capacity = e._capacity;
    _flow = e._flow;
}

FlowEdge &FlowEdge::operator=(FlowEdge &&e) {
    if (this != &e) {
        _v = e._v;
        _w = e._w;
        _capacity = e._capacity;
        _flow = e._flow;
    }
    return *this;
}

FlowEdge &FlowEdge::operator=(FlowEdge &e) {
    if (this != &e) {
        _v = e._v;
        _w = e._w;
        _capacity = e._capacity;
        _flow = e._flow;
    }
    return *this;
}

int FlowEdge::from() const {
    return _v;
}

int FlowEdge::to() const {
    return _w;
}

double FlowEdge::capacity() const {
    return _capacity;
}

double FlowEdge::flow() const {
    return _flow;
}

int FlowEdge::other(int vertex) const {
    if (vertex == _v) return _w;
    else if (vertex == _w) return _v;
    else throw invalid_argument("invalid vertex");
}

double FlowEdge::residualCapacityTo(int vertex) const {
    if (vertex == _v) return _flow;
    else if (vertex == _w) return _capacity - _flow;
    else throw invalid_argument("invalid vertex");
}

void FlowEdge::addResidualFlowTo(int v, double delta) {
    if (delta < 0.0) {
        throw invalid_argument("delta must be nonnegative");
    }
    if (v == _v) _flow -= delta;
    else if (v == _w) _flow += delta;
    else throw invalid_argument("invalid vertex");
    if (abs(_flow) <= FLOATING_POINT_EPSILON) _flow = 0.0;
    if (abs(_flow - _capacity) <= FLOATING_POINT_EPSILON) _flow = _capacity;
    if (_flow < 0.0) {
        throw invalid_argument("flow is negative");
    }
    if (_flow > _capacity) {
        throw invalid_argument("flow exceeds capacity");
    }
}

string FlowEdge::toString() const {
    stringstream ss;
    ss << _v << "->" << _w << " " << _flow << "/" << _capacity;
    return ss.str();
}

ostream &operator<<(ostream &os, const FlowEdge &e) {
    os << e.toString();
    return os;
}

class CapacityComparator {
public:
    bool operator()(const FlowEdge& e1, const FlowEdge& e2) const {
        return e1.capacity() < e2.capacity();
    }
};
class FlowComparator {
public:
    bool operator()(const FlowEdge& e1, const FlowEdge& e2) const {
        return e1.flow() < e2.flow();
    }
};


#endif //FLOWNETWORK_FLOWEDGE_H

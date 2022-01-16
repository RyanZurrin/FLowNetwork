#include <iostream>
#include "FordFulkerson.h"

int main() {
    int V = 500;
    int E = 4000;
    int s = 0;
    int t = V - 1;
    FlowNetwork flowNetwork(V, E, s, t, true, true);


    FordFulkerson fordFulkerson(flowNetwork);
    cout << fordFulkerson << endl;


    return EXIT_SUCCESS;
}

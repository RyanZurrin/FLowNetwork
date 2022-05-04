#include <iostream>
#include "FordFulkerson.h"

int main() {
    // open a file and use istream to read the data
    std::ifstream in("test2.txt");
    std::istream &is = in;
    // create a graph
    FlowNetwork flowNetwork(is, 0, 7);

    FordFulkerson fordFulkerson(flowNetwork);
    cout << fordFulkerson << endl;

    // make a random flow network of 15 vertices and 40 edges
    FlowNetwork flowNetwork1(15, 40, 0, 14);

    FordFulkerson fordFulkerson1(flowNetwork1);
    cout << fordFulkerson1 << endl;





    return EXIT_SUCCESS;
}

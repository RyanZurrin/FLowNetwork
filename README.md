# FLowNetwork

to use clone the repo and then build your graph in a text file in the following format: 
> num_vertices num_edges<br/>
> from -> to N where N is the capacity of the edge
```
8 15
0 1 10
0 3 5
0 5 15
1 2 9
1 3 4
1 4 15
2 4 15
2 7 10
3 4 8
3 5 4
4 6 15
4 7 10
5 6 16
6 3 6
6 7 10
```
# Sample code to read a graph from a file
``` c++
int main() {
    // open a file and use istream to read the data
    std::ifstream in("test2.txt");
    std::istream &is = in;
    // create a graph
    FlowNetwork flowNetwork(is, 0, 7);

    FordFulkerson fordFulkerson(flowNetwork);
    cout << fordFulkerson << endl;

    return EXIT_SUCCESS;
}
```

# Sample code to build a random graph
``` c++
int main() {
    // make a random flow network of 15 vertices and 40 edges
    FlowNetwork flowNetwork1(15, 40, 0, 14);

    FordFulkerson fordFulkerson1(flowNetwork1);
    cout << fordFulkerson1 << endl;

    return EXIT_SUCCESS;
}
```

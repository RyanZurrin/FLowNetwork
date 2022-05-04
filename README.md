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
file saved as test2.txt for example below
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
## output
>*** Flow is optimal ***<br/>
Max flow from  0 to 7<br/>
(0->5 13/15)  (0->3 5/5)  (0->1 10/10)  (1->4 2/15)  (1->2 8/9)  (2->7 8/10)<br/>  (3->4 8/8)  (4->7 10/10)
(5->6 13/16)  (6->7 10/10)  (6->3 3/6)<br/><br/>Min s-t cut:<br/>
(0 - 1), (3 - 4), (6 - 7)<br/><br/>
Max flow value = 28.000000

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
## output
>*** Flow is optimal ***<br/>
Max flow from  0 to 14<br/>
(0->8 56/58)  (0->7 36/66)  (6->14 14/14)  (6->11 27/76)  (7->11 36/36)  (8->6 41/98)<br/>
(8->14 15/15)  (11->14 63/63)<br/><br/>
Min s-t cut:<br/> (6 - 14), (8 - 14), (11 - 14)<br/><br/>
Max flow value = 92.000000
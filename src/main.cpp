#include <iostream>
#include <string> 

#include <config.h>
#include "graph.h"

using namespace std; 

int main(int argc, char * argv[])
{
    std::cout << "Hello, World!" << std::endl;
    std::cout << "creating graph ..." << endl; 
    //test(); 
    
    Graph<std::string> graph; 
    graph.addVertice("s1"); 
    graph.addVertice("s2"); 
    graph.addVertice("s3"); 
    graph.addVertice("s4"); 
    graph.addVertice("s5"); 
    graph.addVertice("s6"); 
    graph.addEdge("s1", "s4", 10); 
    graph.addEdge("s2", "s5", 10); 
    graph.addEdge("s3", "s6", 10); 
    cout << "num vertices:" << graph.getNumVertices() << endl; 
    cout << "num edges:" << graph.getNumEdges()  << endl; 
    return 0;
}
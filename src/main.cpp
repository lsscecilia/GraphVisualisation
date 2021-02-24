#include <iostream>
#include <string> 

#include <config.h>
#include "algorithm.h"

using namespace std; 

int main(int argc, char * argv[])
{
    std::cout << "Hello, World!" << std::endl;
    vector<Vertex> vertices; 
    vertices.push_back({{4,1}, {0,0}});
    vertices.push_back({{4.4,0.1}, {0,0}});
    vertices.push_back({{3.2,4.2}, {0,0}});
    vertices.push_back({{3.3,1}, {0,0}});
    vertices.push_back({{1.5,2.4}, {0,0}});
    
    vector<vector<bool>> edges; 
    edges.push_back({false,true,true,true, true}); 
    edges.push_back({true,false,true,true, true}); 
    edges.push_back({true,true,false,true, true}); 
    edges.push_back({true,true,true,false, true}); 
    edges.push_back({true,true,true,true, false}); 

    for (int i=0;i<vertices.size(); i++){
        cout << i << "| pos(" << vertices[i].pos.x << " ," << vertices[i].pos.y << "), disp("
        << vertices[i].disp.x << " ," << vertices[i].disp.y << ")" << endl; 
    }
    
    directedForceAlgorithm(vertices, edges, 5,5,100); 

    for (int i=0;i<vertices.size(); i++){
        cout << i << "| pos(" << vertices[i].pos.x << " ," << vertices[i].pos.y << "), disp("
        << vertices[i].disp.x << " ," << vertices[i].disp.y << ")" << endl; 
    }
    /*
    std::cout << "creating graph ..." << endl; 
    
    graph::Graph<std::string> graph; 
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
    cout << "num edges:" << graph.getNumEdges()  << endl; */
    return 0;
}
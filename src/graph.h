#include <vector>
using namespace std; 

namespace graph{
  template <class T> 
  struct Edge{
    pair<T,T> edge; 
    int weight; 
  }; 

  template <class T>
  class Graph{
    private:
      int numVertices=0; 
      vector<T> vertices;  
      vector<Edge<T>> edges;  
    public:
      Graph(){

      };
      ~ Graph(){
        
      }; 
      void addEdge(T v1, T v2, int w){
        edges.push_back({make_pair(v1,v2),w}); 
      }; 
      void removeEdge(T v1, T v2){
        for (int i=0; i<edges.size(); i++){
          if (edges[i].edge.first == v1){
            if (edges[i].edge.second == v2){
              edges.erase(i);
            }
          }
          else if (edges[i].edge.first == v2){
            if (edges[i].edge.second == v1){
              edges.erase(i); 
            }
        }
    }
      };
      void addVertice(T v1){
        vertices.emplace_back(v1); 
        numVertices++; 
      }; 

      int getNumVertices(){
        return numVertices; 
      }

      int getNumEdges(){
        return edges.size(); 
      }
  }; 
}
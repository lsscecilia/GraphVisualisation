#include <iostream>
#include <vector> 
using namespace std;

template <class T>
class Graph{
  private:
    struct{
      pair<T,T> edge; 
      int weight; 
    }; 
    int numVertices; 
    vector<T> vertices;  
}; 

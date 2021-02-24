#include <vector>

#include "mathVector.h"

using namespace std; 

struct Vertex{
  MathVector pos; 
  MathVector disp; 
}; 

void directedForceAlgorithm(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, int L, int W, int iterations); 
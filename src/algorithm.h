#include <vector>

#include "tree.h"

using namespace std; 

void initVerticesPosition(vector<Vertex>& vertices, double xMax, double yMax); 

void directedForceAlgorithm(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, int L, int W, int iterations); 
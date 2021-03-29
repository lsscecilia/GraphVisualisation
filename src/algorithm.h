#include <vector>

#include "tree.h"

using namespace std; 

void initVerticesPosition(vector<shared_ptr<Vertex>>& vertices, double xMax, double yMax, bool random); 

void directedForceAlgorithm(vector<shared_ptr<Vertex>>& vertices, vector<vector<double>>& adjMax, int L, int W, int iterations, int algoType, double mass = 1, bool dynamic=true); 
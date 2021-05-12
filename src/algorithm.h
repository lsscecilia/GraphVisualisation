#include <vector>

#include "tree.h"

using namespace std; 

void initVerticesPosition(vector<shared_ptr<Vertex>>& vertices, double xMax, double yMax, bool random); 

void directedForceAlgorithm(vector<shared_ptr<Vertex>>& vertices, vector<vector<double>>& adjMax, int L, int W, int iterations, int algoType, double theta, double mass = 1, bool dynamic=true); 

void initVerticesPosition3D(vector<shared_ptr<Vertex3D>>& vertices, double xMax, double yMax, double zMax, bool random); 

void directedForceAlgorithm3D(vector<shared_ptr<Vertex3D>>& vertices, vector<vector<double>>& adjMax, int L, int W, int iterations, int algoType, double theta, double mass = 1, bool dynamic=true); 

#include "algorithm.h"

//k is const
double af(int k, double x){
  return x*x/k; 
}

//k is const
double rf(int k, double z){
  return k*k/z; 
}

double cool(double t){
  if (t > 0.1)
    return t*0.99; 
  else 
    return 0.1; 
}

void directedForceAlgorithm(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, int L, int W, int iterations){
  int numVertices = vertices.size(); 
  int area = W*L;  
  double k = sqrt(area/numVertices); 
  double coeff, t; 

  MathVector diff; 
  double diffABS, abs; 
  //in each iterations
  for (int iter=1; iter<iterations; iter++){
    t = 1;  
    for (int i=0; i<numVertices; i++){
      vertices[i].disp = 0; 
      for (int r=0; r<numVertices; r++){
        if (i==r)
          continue; 
        diff = vertices[i].pos - vertices[r].pos; 
        diffABS = diff.abs(); 
        if (diffABS!=0){
          vertices[i].disp += (diff/diffABS)*rf(k,diffABS) ; 
        }
      }
    }

    for (int i=0; i<numVertices; i++){
      for (int r=0; r<i; r++){
        if (adjMax[i][r]){
          diff = vertices[i].pos - vertices[r].pos; 
          diffABS = diff.abs(); 
          if (diffABS!=0){
            vertices[i].disp -= diff/diffABS*af(k,diffABS); 
            vertices[r].disp += diff/diffABS*af(k,diffABS); 
          }
        }
      }
    }

    for (int i=0; i<vertices.size(); i++){
      abs = vertices[i].disp.abs(); 
      vertices[i].pos += vertices[i].disp/abs * min(abs, t); 
    }

    t = cool(t); 
  }
}
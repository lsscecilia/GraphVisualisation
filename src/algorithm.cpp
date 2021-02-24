#include "algorithm.h"

//k is const
double af(int k, double x){
  return x*x/k; 
}

//k is const
double rf(int k, double z){
  return -k*k/z; 
}

double cool(double t){
  return t*0.99; 
}

void directedForceAlgorithm(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, int L, int W, int iterations){
  int numVertices;  //variables to be set
  int area = W*L; 

  double t; 

  //qn: can use adjMax for attractive force since is btw each pair of vertices

  //vertices are assigned to random initial positions 
  numVertices = vertices.size(); 
  double k = sqrt(area/numVertices); 

  MathVector diff; 
  double diffABS, abs; 
  //in each iterations
  for (int iter=0; iter<iterations; iter++){
    for (int i=0; i<vertices.size(); i++){
      vertices[i].disp = 0; 
      for (int r=0; r<vertices.size(); r++){
        if (r==i){
          //then how
        }
        else if (adjMax[i][r]) {
          diff = vertices[i].pos - vertices[r].pos; 
          diffABS = diff.abs(); 
          vertices[i].disp += (diff/diffABS)*rf(k,diffABS) ; 
        }
      }
      
    }
    for (int i=0; i<vertices.size(); i++){
      for (int r=0; r<vertices.size(); r++){
        if (adjMax[i][r]){
          diff = vertices[i].pos - vertices[r].pos; 
          diffABS = diff.abs(); 
          vertices[i].disp -= diff/diffABS*af(k,diffABS); 
          vertices[r].disp += diff/diffABS*af(k,diffABS); 
        }
      }
    }

    for (int i=0; i<vertices.size(); i++){
      abs = vertices[i].disp.abs(); 
      vertices[i].pos += (vertices[i].disp/abs*vertices[i].disp.min(t)); 
      vertices[i].pos.x = min((double)W/2, max((double)-W/2, vertices[i].pos.x)); 
      vertices[i].pos.y = min((double)L/2, max((double)-L/2, vertices[i].pos.y)); 
    }

    t = cool(t); 
  }
}
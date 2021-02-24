#include "algorithm.h"

//delete later 
#include <iostream>

//k is const
double af(int k, double x){
  return x*x/k; 
}

//k is const
double rf(int k, double z){
  return -k*k/z; 
}

double cool(double t){
  if (t > 1.5)
    return t*0.85; 
  else 
    return 1.5; 
}

void directedForceAlgorithm(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, int L, int W, int iterations){
  int numVertices = vertices.size(); 
  int area = W*L;  
  double k = sqrt(area/numVertices); 
  double coeff, t; 

  //qn: can use adjMax for attractive force since is btw each pair of vertices

  //vertices are assigned to random initial positions 
  cout << "k: " << k << endl; 
  cout << "t: " << t << endl;
  MathVector diff; 
  double diffABS, abs; 
  //in each iterations
  for (int iter=1; iter<iterations; iter++){
    t = 1 - iter /iterations; 
    //print 
    for (int i=0; i<numVertices; i++){
      cout << i << "| pos(" << vertices[i].pos.x << "," << vertices[i].pos.y << ")" << endl; 
    }

    for (int i=0; i<numVertices; i++){
      vertices[i].disp = 0; 
      for (int r=i+1; r<numVertices; r++){
        diff = vertices[i].pos - vertices[r].pos; 
        cout << "diff(" << diff.x << "," << diff.y << ")" << endl; 
        diffABS = diff.abs(); 
        cout << "diff abs" << diffABS << endl; 
        if (diffABS!=0){
          vertices[i].disp += (diff/diffABS)*rf(k,diffABS) ; 
          vertices[r].disp -= (diff/diffABS)*rf(k,diffABS) ; 
        }
      }
    }

    for (int i=0; i<numVertices; i++){
      for (int r=0; r<i; r++){
        if (adjMax[i][r]){
          cout << "edge: " << i << " ----- " << r << endl; 
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

      /*
      if (abs>t){
        coeff = t/abs; 
        vertices[i].disp *= coeff; 
      }
      vertices[i].pos += vertices[i].disp; 
      
      if (abs<1)
        continue; */
      if (abs!=0)
        vertices[i].pos += (vertices[i].disp/abs)*vertices[i].disp.min(t); 
      cout << "idk whats this x: " << vertices[i].pos.x << endl; 
      cout << "idk whats this y: " << vertices[i].pos.y << endl;  
      vertices[i].pos.x = min((double)W/2, max((double)-W/2, vertices[i].pos.x)); 
      vertices[i].pos.y = min((double)L/2, max((double)-L/2, vertices[i].pos.y)); 
    }

    //t = cool(t); 
  }
}
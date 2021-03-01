#include <chrono>
#include <random>

#include "algorithm.h"

#include <iostream>

//k is const
double af(int k, double x){
  return x*x/k; 
}

//k is const
double rf(int k, double z){
  return k*k/z; 
}

double cool(double t){
  if (t > 0.0001)
    return t*0.99; 
  else 
    return 0.0001; 
}

double fRand(double fMin, double fMax){
  //srand((unsigned int)time(NULL)); 
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); 
  srand(seed); 
  double f = (double)rand() / RAND_MAX;
  return fMin + f * (fMax - fMin);
}

void initVerticesPosition(vector<Vertex>& vertices, double xMax, double yMax){
  for (int i=0; i< vertices.size();i++){
    vertices[i].pos.x = fRand(0,xMax); 
    vertices[i].pos.y = fRand(0,yMax); 
    //cerr << vertices[i].pos.x << "," << vertices[i].pos.y << endl; 
  }
}

void calculateForceBruteForce(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, double k){
  MathVector diff; 
  double diffABS, abs; 
  int numVertices = vertices.size(); 
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
}

void insert(Node node, Vertex particle){
  if (node.box.in(particle.pos)){
    if (node.noParticles()){
      node.n = &particle; 
    }
    //add in the update centre of mass and total mass those kind of shit
  }
}

void generateTree(vector<Vertex>& vertices){
  int numVertices = vertices.size(); 
  //init tree
  Node root = {nullptr,nullptr, nullptr, nullptr, nullptr};
  for (int i=0; i<numVertices; i++){
    insert(root,vertices[i]); 
  } 
  

}

void calculateForceBarnesHut(){
  //put into tree
  //then calculate force
}

void directedForceAlgorithm(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, int L, int W, int iterations){
  int numVertices = vertices.size(); 
  int area = W*L;  
  double k = sqrt(area/numVertices); 
  double coeff, t; 

  MathVector diff; 
  double diffABS, abs; 
  //in each iterations
  for (int iter=0; iter<iterations; iter++){
    t = 1;  

    //calculate force
    calculateForceBruteForce(vertices, adjMax, k); 

    for (int i=0; i<vertices.size(); i++){
      
      abs = vertices[i].disp.abs(); 
      //vertices[i].pos += vertices[i].disp/abs; 
      vertices[i].pos += vertices[i].disp/abs * min(abs, t); 
    }

    t = cool(t); 
  }
}


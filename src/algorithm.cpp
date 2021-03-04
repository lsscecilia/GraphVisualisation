#include <chrono>
#include <random>

#include "algorithm.h"

#include <iostream>

using namespace std; 
//k is const
double af(int k, double x){
  return x*x/k; 
}

//k is const
double rf(int k, double z){
  return -k*k/z; 
}

double cool(double t){
  if (t > 0.0001)
    return t*0.99; 
  else 
    return 0.0001; 
}

double fRand(double fMin, double fMax){
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); 
  srand(seed); 
  double f = (double)rand() / RAND_MAX;
  return fMin + f * (fMax - fMin);
}

void initVerticesPosition(vector<Vertex>& vertices, double xMax, double yMax){
  for (int i=0; i< vertices.size();i++){
    vertices[i].pos.x = fRand(0,xMax); 
    vertices[i].pos.y = fRand(0,yMax); 
  }
}

void calculateAttrativeForce(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, double k){
  MathVector diff; 
  double diffABS; 
  int numVertices = vertices.size(); 
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
        vertices[i].disp -= (diff/diffABS)*rf(k,diffABS) ; 
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

void insert(Node& node, Vertex& particle){
  if (node.box.in(particle.pos)){
    if (node.noParticles()){
      node.n = &particle; 
    }
    else{
      //node have children
      //get quadrant
      if (node.n!=nullptr){ 
        Node* nQuadrant = node.getQuadrant(node.n->pos); 
        if (nQuadrant->noParticles()){
          nQuadrant->n = node.n; 
        }
        else{
          insert(*nQuadrant, *node.n); 
        }  
        node.n = nullptr;  
      }
      Node* quadrant = node.getQuadrant(particle.pos);
      if (quadrant->noParticles()){
        quadrant->n = &particle; 
      }
      else{
        insert(*quadrant, particle); 
      }
    }
  }
}

Node generateTree(vector<Vertex>& vertices, double width, double length){
  int numVertices = vertices.size(); 
  //init tree
  Node root = {nullptr,nullptr, nullptr, nullptr, nullptr, {{0,0}, {width,0}, {width,length}, {0,length}}};
  for (int i=0; i<numVertices; i++){
    insert(root, vertices[i]); 
  } 
  return root; 
}

void computeMassDistribution(Node* node){
  if (node->numChild()==0 && node->n!=nullptr){
    node->mass = 1; 
    node->centreOfMass.x = node->n->pos.x; 
    node->centreOfMass.y = node->n->pos.y; 
  }
  else{
    if (node->first!=nullptr&&!node->first->noParticles()){
      computeMassDistribution(node->first); 
      node->mass += node->first->mass; 
      node->centreOfMass += node->first->centreOfMass; 
    }
    if (node->second!=nullptr&&!node->second->noParticles()){
      computeMassDistribution(node->second); 
      node->mass += node->second->mass; 
      node->centreOfMass += node->second->centreOfMass; 
    }
    if (node->third!=nullptr&&!node->third->noParticles()){
      computeMassDistribution(node->third); 
      node->mass += node->third->mass; 
      node->centreOfMass += node->third->centreOfMass; 
    }
    if (node->fourth!=nullptr&&!node->fourth->noParticles()){
      computeMassDistribution(node->fourth); 
      node->mass += node->fourth->mass; 
      node->centreOfMass += node->fourth->centreOfMass; 
    }
    node->centreOfMass /=  node->mass; 
  }
} 

MathVector calculateForceBarnesHutPerVertex(Node* node, Vertex* targetParticle, double k){
  double distance, height, theta;
  MathVector force = {0,0}; 

  //if it is a leaf
  if (node->numChild()==0&&node->n!=nullptr){
    MathVector diff = node->n->pos-targetParticle->pos; 
    distance = diff.abs(); 
    if (distance==0)
      return {0,0}; 
    height = node->box.c2.x - node->box.c1.x ; 
    force = (diff/distance)*node->mass*rf(k,distance);
  }
  else{
    MathVector diff = node->centreOfMass-targetParticle->pos; 
    distance = diff.abs(); 
    height = node->box.c2.x - node->box.c1.x ; 
    theta = height/distance; 
      
    if (theta < 0.5){
      force = (diff/distance)*node->mass*rf(k,distance);
    }
    else{
      if (node->first!=nullptr){
        force += calculateForceBarnesHutPerVertex(node->first, targetParticle, k); 
      }
      if (node->second!=nullptr){
        force += calculateForceBarnesHutPerVertex(node->second, targetParticle, k); 
      }
      if (node->third!=nullptr){
        force += calculateForceBarnesHutPerVertex(node->third, targetParticle, k); 
      }
      if (node->fourth!=nullptr){
        force += calculateForceBarnesHutPerVertex(node->fourth, targetParticle, k); 
      }
    }
  }
  return force; 
}

void calculateRepulsiveForce_barnesHutAlgo(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, double k, double width, double length){
  MathVector diff, force; 
  double diffABS, abs; 
  int numVertices = vertices.size(); 
  //generate tree
  Node tree = generateTree(vertices, width, length); 
  computeMassDistribution(&tree); 
  for (int i=0; i<numVertices; i++){
    force = calculateForceBarnesHutPerVertex(&tree, &vertices[i], k); 
    vertices[i].disp = force; 
  }
  tree.deleteTree();
}

void calculateForceBarnesHut(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, double k, double width, double length){ 
  calculateRepulsiveForce_barnesHutAlgo(vertices,adjMax, k, width,length); 
  calculateAttrativeForce(vertices,adjMax,k); 
}

void directedForceAlgorithm(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, int L, int W, int iterations, int algoType){
  int numVertices = vertices.size(); 
  int area = W*L;  
  double k = sqrt(area/numVertices); 
  double coeff, t; 

  MathVector diff; 
  double diffABS, abs; 
  //in each iterations
  
  t = 1; 
  for (int iter=0; iter<iterations; iter++){
    if (algoType==1){
      calculateForceBruteForce(vertices, adjMax, k); 
    }
    else{
      //by default
      calculateForceBarnesHut(vertices, adjMax,k,W,L); 
    }

    //for both different algorithm, you will need this
    for (int i=0; i<vertices.size(); i++){      
      abs = vertices[i].disp.abs(); 
      vertices[i].pos += vertices[i].disp/abs * min(abs, t); 
    }

    t = cool(t); 
  }
}


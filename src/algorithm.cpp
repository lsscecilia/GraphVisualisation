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
  return k*k/z; 
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

void insert(Node& node, Vertex& particle){
  cout << "start of insert.." << endl; 
  if (node.box.in(particle.pos)){
    cout << "particle in box" << endl; 
    if (node.noParticles()){
      cout << "no particle, assign particle to node " << endl; 
      node.n = &particle; 
      cout << &particle << endl; 
      cout << node.n << endl; 
      cout << node.n->pos.x << "," << node.n->pos.y << endl; 
    }
    else{
      cout << "node hav particles" << endl; 
      //node have children
      //get quadrant
      if (node.n!=nullptr){
        cout << "n is not nullptr" << endl; 
        Node* nQuadrant = node.getQuadrant(node.n->pos); 
        if (nQuadrant->noParticles()){
          nQuadrant->n = node.n; 
        }
        else{
          insert(*nQuadrant, *node.n); 
        }
        node.n = nullptr; 
      }
      cout << "deal with particle" << endl;
      Node* quadrant = node.getQuadrant(particle.pos);
    
      if (quadrant->noParticles()){
        cout << "particle quadrant got no particles" << endl; 
        quadrant->n = &particle; 
      }
      else{
        cout << "insert again...." << endl; 
        insert(*quadrant, particle); 
      }
    }
    //add in the update centre of mass and total mass those kind of shit
  }
  else {
    cout << "box" << endl; 
    cout << node.box.c1.x << "," << node.box.c1.y << endl; 
    cout << node.box.c2.x << "," << node.box.c2.y << endl; 
    cout << node.box.c3.x << "," << node.box.c3.y << endl; 
    cout << node.box.c4.x << "," << node.box.c4.y << endl; 
    cout << "particle pos" << particle.pos.x << "," << particle.pos.y << endl; 
  }
}

Node generateTree(vector<Vertex>& vertices, double width, double length){
  int numVertices = vertices.size(); 
  //init tree
  Node root = {nullptr,nullptr, nullptr, nullptr, nullptr, {{0,0}, {width,0}, {width,length}, {0,length}}};
  for (int i=0; i<numVertices; i++){
    cerr << i << "loop" << endl; 
    insert(root, vertices[i]); 
    cerr << root.first << endl; 
    cerr << root.second << endl;
    cerr << root.third << endl;
    cerr << root.fourth << endl;
    cerr << "current n" << root.n << endl; 
  } 

  cerr << "no particles? in generate tree function" << root.noParticles() << endl;
  return root; 
}

void printTree(Node node){
  cout << "print tree" << endl; 
  if(!node.noParticles()){
    if (node.first!=nullptr)
      printTree(*node.first);
    if (node.second!=nullptr)
    printTree(*node.second);
    if (node.n!=nullptr)
      cerr << node.n->pos.x << " "<<node.n->pos.y << endl;
    else
      cerr << "empty" << endl;
    if (node.third!=nullptr)
      printTree(*node.third);
    if (node.fourth!=nullptr)
      printTree(*node.fourth);
  }
}

void calculateForceBarnesHut(Node node, Vertex& particle, double theta){ 
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

/*
int main(){
  vector<Vertex> vertices; 
	vertices.push_back({{1,1},{0,0}}); 
	vertices.push_back({{2,2},{0,0}}); 
	vertices.push_back({{4,3},{0,0}}); 
	vertices.push_back({{5,5},{0,0}}); 
  Node tree = generateTree(vertices); 
  cerr << "no particles? " << tree.noParticles() << endl; 
  printTree(tree); 
}*/


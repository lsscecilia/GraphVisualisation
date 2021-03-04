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
        cerr << "n quad: " <<  nQuadrant << endl; 
        if (nQuadrant->noParticles()){
          nQuadrant->n = node.n; 
          cerr << "n quad no particle... & hence insert into n quad->n" << endl;  
        }
        else{
          insert(*nQuadrant, *node.n); 
        }
        cerr << "before" << nQuadrant->n << endl;   
        node.n = nullptr; 
        cerr << "after" << nQuadrant->n << endl; 
      }
      cout << "deal with particle" << endl;
      Node* quadrant = node.getQuadrant(particle.pos);
      cerr << "quad: " <<  quadrant << endl; 
      cerr << "box:" << quadrant->box.c1.x << "," << quadrant->box.c1.y << endl; 
      cerr << "box:" << quadrant->box.c3.x << "," << quadrant->box.c3.y << endl; 
      cerr << "n" << quadrant->n << endl; 
      cerr << quadrant->first << endl;  
      cerr << quadrant->second << endl;  
      cerr << quadrant->third << endl;  
      cerr << quadrant->fourth << endl;  
      if (quadrant->noParticles()){
        cout << "particle quadrant got no particles" << endl; 
        quadrant->n = &particle; 
      }
      else{
        cout << "insert again...." << particle.pos.x << "," << particle.pos.y<< endl; 
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

void computeMassDistribution(Node* node){
  if (node->numChild()==0 && node->n!=nullptr){
    node->mass = 1; 
    node->centreOfMass.x = node->n->pos.x; 
    node->centreOfMass.y = node->n->pos.y; 
    cerr << "centre of mass in base case.." << node->centreOfMass.x << "," << node->centreOfMass.y << endl;
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
    cerr << "centre of mass" << node->centreOfMass.x << "," << node->centreOfMass.y << endl;
    cerr << "mass" << node->mass << endl; 
  }
} 

MathVector calculateForceBarnesHutPerVertex(Node* node, Vertex* targetParticle, double k){
  double distance, height, theta;
  MathVector force = {0,0}; 

  //if it is a leaf
  if (node->numChild()==0&&node->n!=nullptr){
    MathVector diff = node->n->pos-targetParticle->pos; 
    //MathVector diff = node->centreOfMass-targetParticle->pos; 
    distance = diff.abs(); 
    if (distance==0)
      return {0,0}; 
    cerr << "base case distance" << distance << endl; 
    height = node->box.c2.x - node->box.c1.x ; 
    cerr << "height" << height << endl; 
    force = (diff/distance)*node->mass*rf(k,distance);
    cerr << "bsae case force..." << force.x <<"," << force.y << endl; 
  }
  else{
    MathVector diff = node->centreOfMass-targetParticle->pos; 
    distance = diff.abs(); 
    height = node->box.c2.x - node->box.c1.x ; 
    theta = height/distance; 
    cerr << "theta: " << theta << endl;  
    //isit really 0?  
    if (theta < 0.5){
      force = (diff/distance)*node->mass*rf(k,distance);
    }
    else{
      if (node->first!=nullptr){
        force += calculateForceBarnesHutPerVertex(node->first, targetParticle, k);
        cerr << "aft first node force .." << force.x << "," << force.y << endl;  
      }
      if (node->second!=nullptr){
        force += calculateForceBarnesHutPerVertex(node->second, targetParticle, k); 
        cerr << "aft second node force .." << force.x << "," << force.y << endl; 
      }
      if (node->third!=nullptr){
        force += calculateForceBarnesHutPerVertex(node->third, targetParticle, k); 
        cerr << "aft third node force .." << force.x << "," << force.y << endl; 
      }
      if (node->fourth!=nullptr){
        force += calculateForceBarnesHutPerVertex(node->fourth, targetParticle, k); 
        cerr << "aft fourth node force .." << force.x << "," << force.y << endl; 
      }
    }
  }
  cerr << "force..." << force.x <<"," << force.y << endl; 
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
}

void calculateForceBarnesHut(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, double k, double width, double length){ 
  calculateRepulsiveForce_barnesHutAlgo(vertices,adjMax, k, width,length); 
  calculateAttrativeForce(vertices,adjMax,k); 
}

void directedForceAlgorithm(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, int L, int W, int iterations){
  int numVertices = vertices.size(); 
  int area = W*L;  
  double k = sqrt(area/numVertices); 
  double coeff, t; 

  MathVector diff; 
  double diffABS, abs; 
  //in each iterations
  
  t = 1; 
  for (int iter=0; iter<iterations; iter++){
    //calculate force
    //calculateForceBruteForce(vertices, adjMax, k); 

    //barnes hut 
    
    calculateForceBarnesHut(vertices, adjMax,k,W,L); 


    //for both different algorithm, you will need this
    for (int i=0; i<vertices.size(); i++){      
      abs = vertices[i].disp.abs(); 
      //vertices[i].pos += vertices[i].disp/abs; 
      vertices[i].pos += vertices[i].disp/abs * min(abs, t); 
    }

    t = cool(t); 
  }
}


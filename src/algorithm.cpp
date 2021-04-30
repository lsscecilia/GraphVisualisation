#include <chrono>
#include <random>
#include <memory>
#include <math.h>  
#include <cmath>  

#include "algorithm.h"
#include "progressBar.h"

#include <iostream>

using namespace std; 
//k is const
double af(double k, double x){
  double k2 = k; 
  return x*x/k2; 
}

double af(double k, double x, double weight){
  double k2 = k; 
  return x*x/k2*weight;
}

//k is const
double rf(double k, double z){
  double k2 = k; 
  return -k2*k2/z; 
}

double getConstC(double width){
  double c = width/10; 
  return c; 
} 

double cool(double t){
  if (t > 0.001)
    return t*0.99; 
  else 
    return 0.001; 
}

double fRand(double fMin, double fMax){
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); 
  srand(seed); 
  double f = (double)rand() / RAND_MAX;
  return fMin + f * (fMax - fMin);
}

void initVerticesPosition(vector<shared_ptr<Vertex>>& vertices, double xMax, double yMax, bool random){
  if (random){
    for (int i=0; i< vertices.size();i++){
      vertices[i]->pos.x = fRand(0,xMax); 
      vertices[i]->pos.y = fRand(0,yMax); 
    }
  }
  else{
    int numV = vertices.size(); 
    int iter=1; 
    double angle;

    angle = 2.0 * M_PI / numV;
    for (int i=0;i<numV;i++){
      vertices[i]->pos.x = cos(angle*i); 
      vertices[i]->pos.y = sin(angle*i); 
    }
  }
}

void calculateAttrativeForce(vector<shared_ptr<Vertex>>& vertices, vector<vector<double>>& adjMax, double k){
  MathVector diff; 
  double diffABS; 
  int numVertices = vertices.size(); 
  for (int i=0; i<numVertices; i++){
    for (int r=0; r<i; r++){
      if (adjMax[i][r]>0){
        diff = vertices[i]->pos - vertices[r]->pos; 
        diffABS = diff.abs(); 
        if (diffABS!=0){
          vertices[i]->disp -= diff/diffABS*af(k,diffABS,adjMax[i][r]); 
          vertices[r]->disp += diff/diffABS*af(k,diffABS,adjMax[i][r]); 
        }
      }
    }
  }
}

void calculateForceBruteForce(vector<shared_ptr<Vertex>>& vertices, vector<vector<double>>& adjMax, double k){
  MathVector diff; 
  double diffABS, abs; 
  int numVertices = vertices.size(); 
  for (int i=0; i<numVertices; i++){
    vertices[i]->disp = 0; 
    for (int r=0; r<numVertices; r++){
      if (i==r)
        continue; 
      diff = vertices[i]->pos - vertices[r]->pos; 
      diffABS = diff.abs(); 
      if (diffABS!=0){
        vertices[i]->disp -= (diff/diffABS)*rf(k,diffABS) ; 
      }else{
        vertices[i]->disp -= rf(k,10) ; 
      }
    }
  }

  for (int i=0; i<numVertices; i++){
    for (int r=0; r<i; r++){
      if (adjMax[i][r]>0){
        diff = vertices[i]->pos - vertices[r]->pos; 
        diffABS = diff.abs(); 
        if (diffABS!=0){
          vertices[i]->disp -= diff/diffABS*af(k,diffABS,adjMax[i][r]); 
          vertices[r]->disp += diff/diffABS*af(k,diffABS,adjMax[i][r]); 
        }
      }
    }
  }
}

bool insert(shared_ptr<Node>& node, shared_ptr<Vertex>& particle){
  if (node->box.in(particle->pos)){
    if (node->noParticles()){
      node->n = particle;  
    }
    else{
      //node have children
      //get quadrant
      if (node->n!=nullptr){ 
        if (node->n->pos.x == particle->pos.x && node->n->pos.y == particle->pos.y){
          particle->pos += (node->box.c2 - node->box.c1)*2; 
          return false; 
        }
        shared_ptr<Node>& nQuadrant = node->getQuadrant(node->n->pos); 
        if (nQuadrant->noParticles()){
          nQuadrant->n = node->n; 
        }
        else{
          insert(nQuadrant, node->n); 
        }  
        node->n = nullptr;  
      }
      shared_ptr<Node>& quadrant = node->getQuadrant(particle->pos);
      if (quadrant->noParticles()){
        quadrant->n = particle; 
      }
      else{
        insert(quadrant, particle); 
      }
    }
  }
  else{
    cerr << "[GraphVisualisation::WARNING] Increase width/length" << endl; 
  }
  return true; 
}

Box getBoundingBox(vector<shared_ptr<Vertex>>& vertices){
  double xMin=0, yMin=0, xMax=0, yMax=0; 
  for (int i=0;i<vertices.size();i++){
    if (vertices[i]->pos.x < xMin){
      xMin = vertices[i]->pos.x; 
    }
    if (vertices[i]->pos.x > xMax){
      xMax = vertices[i]->pos.x; 
    }

    if (vertices[i]->pos.y < yMin){
      yMin = vertices[i]->pos.y; 
    }
    
    if (vertices[i]->pos.y > yMax){
      yMax = vertices[i]->pos.y; 
    }
  }
  Box box = {{xMin,yMin}, {xMax,yMin},{xMax,yMax}, {xMin,yMax}}; 
  return box; 
}

void initTree(shared_ptr<Node>& root, double width, double length,vector<shared_ptr<Vertex>>& vertices, bool dynamic){
  root->first = nullptr; 
  root->second = nullptr;
  root->third = nullptr; 
  root->fourth = nullptr; 
  root->n = nullptr;
  
  if (dynamic){
    root->box = getBoundingBox(vertices);     
  }
  else{
    root->box = {{0,0}, {width,0}, {width,length}, {0,length}}; 
  }
}

void generateTree(vector<shared_ptr<Vertex>>& vertices, double width, double length, shared_ptr<Node>& root, bool dynamic){
 
  int numVertices = vertices.size(); 
  //init tree
  initTree(root,width,length, vertices, dynamic); 
  for (int i=0; i<numVertices; i++){
    auto result = insert(root, vertices[i]); 
    while (!result){
      vertices[i]->pos.x = fRand(0,width); 
      vertices[i]->pos.y = fRand(0,length); 
      result = insert(root, vertices[i]); 
    }
  } 
}

void computeMassDistribution(shared_ptr<Node>& node, double mass=1){
  if (node->numChild()==0 && node->n!=nullptr){
    node->mass = mass;
    node->centreOfMass.x = node->n->pos.x; 
    node->centreOfMass.y = node->n->pos.y; 
  }
  else{
    if (node->first!=nullptr&&!node->first->noParticles()){
      computeMassDistribution(node->first, mass); 
      node->mass += node->first->mass; 
      node->centreOfMass += node->first->centreOfMass; 
    }
    if (node->second!=nullptr&&!node->second->noParticles()){
      computeMassDistribution(node->second, mass); 
      node->mass += node->second->mass; 
      node->centreOfMass += node->second->centreOfMass; 
    }
    if (node->third!=nullptr&&!node->third->noParticles()){
      computeMassDistribution(node->third, mass); 
      node->mass += node->third->mass; 
      node->centreOfMass += node->third->centreOfMass; 
    }
    if (node->fourth!=nullptr&&!node->fourth->noParticles()){
      computeMassDistribution(node->fourth, mass); 
      node->mass += node->fourth->mass; 
      node->centreOfMass += node->fourth->centreOfMass;  
    }
    node->centreOfMass /=  node->mass; 
  }
} 

MathVector calculateForceBarnesHutPerVertex(shared_ptr<Node>& node, shared_ptr<Vertex>& targetParticle, double k, double theta_const){
  double distance, height, theta;
  MathVector force = {0,0}; 

  //if it is a leaf
  if (node->numChild()==0&&node->n!=nullptr){
    MathVector diff = node->centreOfMass-targetParticle->pos;
    distance = diff.abs(); 
    
    if (distance==0){
      return {0,0};
    }
    height = node->box.c2.x - node->box.c1.x ; 
    force = (diff/distance)*(node->mass*rf(k,distance));
  }
  else{
    MathVector diff = node->centreOfMass-targetParticle->pos; 
    distance = diff.abs(); 
    height = node->box.c2.x - node->box.c1.x ; 
    theta = height/distance; 

    if (distance==0){
      return {0,0};
    }
       
    if (distance!=distance){
      std::cerr << "[GraphVisualisation::Warning] W & L must be a bigger number" << endl; 
      return {0,0}; 
    }
    if (theta < theta_const){
      auto temp = diff/distance; 
      force = (diff/distance)*(node->mass*rf(k,distance));
    }
    else{
      if (node->first!=nullptr){
        force += calculateForceBarnesHutPerVertex(node->first, targetParticle, k, theta_const); 
      }
      if (node->second!=nullptr){
        force += calculateForceBarnesHutPerVertex(node->second, targetParticle, k, theta_const); 
      }
      if (node->third!=nullptr){
        force += calculateForceBarnesHutPerVertex(node->third, targetParticle, k, theta_const); 
      }
      if (node->fourth!=nullptr){
        force += calculateForceBarnesHutPerVertex(node->fourth, targetParticle, k, theta_const); 
      }
    }
  }
  return force; 
}

void calculateRepulsiveForce_barnesHutAlgo(vector<shared_ptr<Vertex>>& vertices, vector<vector<double>>& adjMax, double k, double width, double length, double mass, bool dynamic, double theta){
  MathVector diff, force; 
  double diffABS, abs; 
  int numVertices = vertices.size(); 
  //generate tree
  shared_ptr<Node> tree = make_shared<Node>();
  generateTree(vertices, width, length, tree, dynamic); 
  computeMassDistribution(tree, mass);
  for (int i=0; i<numVertices; i++){
    force = calculateForceBarnesHutPerVertex(tree, vertices[i], k, theta); 
    vertices[i]->disp = force; 
  }
}

void calculateForceBarnesHut(vector<shared_ptr<Vertex>>& vertices, vector<vector<double>>& adjMax, double k, double width, double length, double mass, bool dynamic, double theta){ 
  calculateRepulsiveForce_barnesHutAlgo(vertices,adjMax, k, width,length, mass, dynamic, theta); 
  calculateAttrativeForce(vertices,adjMax,k); 
}

void directedForceAlgorithm(vector<shared_ptr<Vertex>>& vertices, vector<vector<double>>& adjMax, int L, int W, int iterations, int algoType, double theta, double mass, bool dynamic){
  int numVertices = vertices.size(); 
  int area = W*L;  
  //by right should be area/numVertices
  //double k = 1;
  double k = sqrt(area/numVertices); 
  double coeff, t; 

  MathVector diff; 
  double diffABS, abs; 

  ProgressBar bar;
  bar.set_bar_width(50);
  bar.fill_bar_progress_with("■");
  bar.fill_bar_remainder_with(" ");
  float progress;
  //in each iterations
  t = 1; 
  for (int iter=1; iter<=iterations; iter++){
    if (algoType==1){
      calculateForceBruteForce(vertices, adjMax, k); 
    }
    else{
      //by default
      calculateForceBarnesHut(vertices, adjMax,k,W,L,mass, dynamic, theta); 
    }

    //for both different algorithm, you will need this
    for (int i=0; i<vertices.size(); i++){      
      abs = vertices[i]->disp.abs(); 
      vertices[i]->pos += vertices[i]->disp/abs * min(abs, t); 
    }

    t = cool(t); 
    // progress bar
    progress = ((double) iter/iterations )* 100;
    bar.update(progress);
  }
  std::cerr << std::endl;
}


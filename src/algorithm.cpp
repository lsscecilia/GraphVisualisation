#include <chrono>
#include <random>
#include <memory>

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
  if (t > 0.001)
    return t*0.999; 
  else 
    return 0.001; 
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

void insert(shared_ptr<Node>& node, shared_ptr<Vertex>& particle){
  if (node->box.in(particle->pos)){
    //cerr << "start insert particle: "<< particle->pos.x <<"," <<particle->pos.y << endl; 
    if (node->noParticles()){
      //cerr << "here is the prob?" << endl; 
      node->n = particle;  
    }
    else{
      //node have children
      //get quadrant
      if (node->n!=nullptr){ 
        shared_ptr<Node>& nQuadrant = node->getQuadrant(node->n->pos); 
        if (nQuadrant->noParticles()){
          //cerr << "if" << endl;
          nQuadrant->n = node->n; 
        }
        else{
          //cerr << "else" << endl; 
          insert(nQuadrant, node->n); 
        }  
        //cerr << "here got prob? " << endl;
        node->n = nullptr;  
        //cerr << "*****" << endl;  
      }
      shared_ptr<Node>& quadrant = node->getQuadrant(particle->pos);
      if (quadrant->noParticles()){
        //cerr << "if.." << endl; 
        quadrant->n = particle; 
      }
      else{
        //cerr << "else.." << endl; 
        insert(quadrant, particle); 
      }
    }
  }
  else{
    cerr << "it is out of the box" << endl; 
  }
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

void initTree(shared_ptr<Node>& root, double width, double length,vector<shared_ptr<Vertex>>& vertices){
  root->first = nullptr; 
  root->second = nullptr;
  root->third = nullptr; 
  root->fourth = nullptr; 
  root->n = nullptr;

  //dynamic for box
  root->box = getBoundingBox(vertices);  

  //static box on width & length
  //root->box = {{0,0}, {width,0}, {width,length}, {0,length}}; 
}

void generateTree(vector<shared_ptr<Vertex>>& vertices, double width, double length, shared_ptr<Node>& root){
  //cerr << "the tree size BY RIGHTTTT: " << vertices.size() << endl; 
 
  int numVertices = vertices.size(); 
  //init tree
  initTree(root,width,length, vertices); 
  //root = {nullptr,nullptr, nullptr, nullptr, nullptr, {{0,0}, {width,0}, {width,length}, {0,length}}};
  //cerr << "number of iterations" << endl;
  for (int i=0; i<numVertices; i++){
    //cerr << i << " iterations=============" << endl; 
    //cerr << "start insert init tree" << endl; 
    insert(root, vertices[i]); 
    //cerr << "wat is gg on here" << i << endl; 
    //cerr << "end insert init tree" << endl;
  } 
}

void computeMassDistribution(shared_ptr<Node>& node){
  //cerr << "any particle: " << node->noParticles() <<endl; 
  //cerr << "COMPUTE MASS DISTRIBUTION" << endl; 
  if (node->numChild()==0 && node->n!=nullptr){
    //cerr << "if" << endl; 
    node->mass = 1;
    node->centreOfMass.x = node->n->pos.x; 
    node->centreOfMass.y = node->n->pos.y; 
    //cerr << "compute mass distribution:" << node->mass << "|centre of mass " <<  
    //node->centreOfMass.x << "," << node->centreOfMass.y << endl; 
  }
  else{
    //cerr << "else" << endl; 
    if (node->first!=nullptr&&!node->first->noParticles()){
      computeMassDistribution(node->first); 
      node->mass += node->first->mass; 
      node->centreOfMass += node->first->centreOfMass; 
      //cerr << "1. compute mass distribution:" << node->mass << "|centre of mass " <<  
      //node->centreOfMass.x << "," << node->centreOfMass.y << endl; 
    }
    if (node->second!=nullptr&&!node->second->noParticles()){
      computeMassDistribution(node->second); 
      node->mass += node->second->mass; 
      node->centreOfMass += node->second->centreOfMass; 
      //cerr << "2. compute mass distribution:" << node->mass << "|centre of mass " <<  
      //node->centreOfMass.x << "," << node->centreOfMass.y << endl; 
    }
    if (node->third!=nullptr&&!node->third->noParticles()){
      computeMassDistribution(node->third); 
      node->mass += node->third->mass; 
      node->centreOfMass += node->third->centreOfMass; 
     // cerr << "3. compute mass distribution:" << node->mass << "|centre of mass " <<  
      //node->centreOfMass.x << "," << node->centreOfMass.y << endl; 
    }
    if (node->fourth!=nullptr&&!node->fourth->noParticles()){
      computeMassDistribution(node->fourth); 
      node->mass += node->fourth->mass; 
      node->centreOfMass += node->fourth->centreOfMass; 
      //cerr << "4. compute mass distribution:" << node->mass << "|centre of mass " <<  
      //node->centreOfMass.x << "," << node->centreOfMass.y << endl; 
    }
    node->centreOfMass /=  node->mass; 
    //cerr << "final compute mass distribution:" << node->mass << "|centre of mass " <<  
    //node->centreOfMass.x << "," << node->centreOfMass.y << endl; 
  }
  //cerr << "end" << endl ; 
} 

MathVector calculateForceBarnesHutPerVertex(shared_ptr<Node>& node, shared_ptr<Vertex> targetParticle, double k){
  double distance, height, theta;
  MathVector force = {0,0}; 

  //if it is a leaf
  if (node->numChild()==0&&node->n!=nullptr){
    MathVector diff = node->centreOfMass-targetParticle->pos;
    //MathVector diff = node->n->pos-targetParticle->pos; 
    distance = diff.abs(); 
    if (distance==0)
      return {0,0}; 
    height = node->box.c2.x - node->box.c1.x ; 
    force = (diff/distance)*node->mass*rf(k,distance);
  }
  else{
    //cerr << "target particle position: " << targetParticle->pos.x << "," << targetParticle->pos.y << endl;
    //cerr << "centre of mass " << node->centreOfMass.x << "," << node->centreOfMass.y << endl; 
    MathVector diff = node->centreOfMass-targetParticle->pos; 
    //cerr << "diff.." << diff.x <<"," <<diff.y << endl; 
    distance = diff.abs(); 
    //cerr << "distance" << distance << endl; 
    height = node->box.c2.x - node->box.c1.x ; 
    theta = height/distance; 
    if (distance!=distance){
      //cerr << "f" << f << endl; 
      //cerr<< "distance is nan" << endl; 
      return {0,0}; 
    }
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

void calculateRepulsiveForce_barnesHutAlgo(vector<shared_ptr<Vertex>>& vertices, vector<vector<bool>>& adjMax, double k, double width, double length){
  MathVector diff, force; 
  double diffABS, abs; 
  int numVertices = vertices.size(); 
  //generate tree
  //cerr << "before generate tree, size:" << numVertices << endl; 
  shared_ptr<Node> tree = make_shared<Node>();
  generateTree(vertices, width, length, tree); 
  //cerr << "start of a iteration: "<< endl;
  //cerr << "before compute mass distribution" << endl; 
  //shared_ptr<Node> temp = std::make_shared<Node>(tree); 
  computeMassDistribution(tree); 

  //cerr << "before calculare force " << endl ; 
  for (int i=0; i<numVertices; i++){
    force = calculateForceBarnesHutPerVertex(tree, vertices[i], k); 
    vertices[i]->disp = force; 
    //cerr << "where the fuck is the dump?" << endl; 
  }
  //cerr << "aft calculate force" << endl;
}

void calculateForceBarnesHut(vector<Vertex>& vertices, vector<vector<bool>>& adjMax, double k, double width, double length){ 
  
  vector<shared_ptr<Vertex>> spVector; 
  for (int i=0; i<vertices.size();i++){
    auto temp = make_shared<Vertex>(vertices[i]); 
    spVector.push_back(temp); 
  }
  
  calculateRepulsiveForce_barnesHutAlgo(spVector,adjMax, k, width,length); 
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
    //cerr << "iteration.." << iter << endl; 
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

      //cerr << "before frame.." << endl; 
      //cerr << "before" << endl; 
      //cerr << vertices[i].pos.x << endl; 
      //cerr << vertices[i].pos.y << endl; 

      /*
      if (vertices[i].pos.x<0 || vertices[i].pos.x > W){
        vertices[i].pos.x = fRand(0,W); 
        cerr << "x go out of bound.." << endl; 
      }

      if (vertices[i].pos.y<0 || vertices[i].pos.y > L){
        vertices[i].pos.y = fRand(0,L); 
        cerr << "y go out of bound.." << endl; 
      }*/
          
      /*
      vertices[i].pos.x = min((double)W-0.001, max((double)0.001, vertices[i].pos.x)); 
      vertices[i].pos.y = min((double)L-0.001, max((double)0.001, vertices[i].pos.y)); */
      //cerr << "dump because of the limit frame shit" << endl; 
     // cerr << "after" << endl; 
      //cerr << vertices[i].pos.x << endl; 
     // cerr << vertices[i].pos.y << endl; 
    }

    t = cool(t); 
  }
}


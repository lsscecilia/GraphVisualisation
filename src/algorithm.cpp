#include <chrono>
#include <random>
#include <memory>
#include <math.h>  
#include <cmath>  

#include "algorithm.h"

#include <iostream>

using namespace std; 
//k is const
double af(double k, double x){
  double k2 = k; 
  return x*x/k2; 
}

//k is const
double rf(double k, double z){
  double k2 = k; 
  //cerr << "k" << k << endl; 
  //cerr << "z" << z << endl; 
  //cerr << "repulsive force" << -k2*k2/z << endl;
  return -k2*k2/z; 
}

double getConstC(double width){
  //assuming width and length the same
  double c = width/10; 
  return c; 
} 

double cool(double t){
  if (t > 0.001)
    return t*0.85; 
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
    //init circle
    int numV = vertices.size(); 
    int iter=1; 
    double angle;

    angle = 2.0 * M_PI / numV;
    for (int i=0;i<numV;i++){
      //cerr << "index:: " << index << endl;
      vertices[i]->pos.x = cos(angle*i); 
      vertices[i]->pos.y = sin(angle*i); 
    }
    /*
    if (numV>100){
      iter = numV/100; 
      angle = 2.0 * M_PI / 100;
    }
    int index=0; 
    for (int it=1;it<=iter;it++){
      for (int i=0;i<100;i++){
        cerr << "index:: " << index << endl;
        vertices[index]->pos.x = it*cos(angle*i); 
        vertices[index]->pos.y = it*sin(angle*i); 
        index++;
      }
    }

    int r= numV%100; 
    angle = 2.0 * M_PI / r;
    for (int i=0;i<r;i++){
      cerr << "index:: " << index << endl;
      vertices[index]->pos.x = (iter+1)*cos(angle*i); 
      vertices[index]->pos.y = (iter+1)*sin(angle*i); 
      index++;
    }*/

  }
}

void calculateAttrativeForce(vector<shared_ptr<Vertex>>& vertices, vector<vector<bool>>& adjMax, double k){
  MathVector diff; 
  double diffABS; 
  int numVertices = vertices.size(); 
  for (int i=0; i<numVertices; i++){
    for (int r=0; r<i; r++){
      if (adjMax[i][r]){
        diff = vertices[i]->pos - vertices[r]->pos; 
        diffABS = diff.abs(); 
        if (diffABS!=0){
          vertices[i]->disp -= diff/diffABS*af(k,diffABS); 
          vertices[r]->disp += diff/diffABS*af(k,diffABS); 
        }
      }
    }
  }
}

void calculateForceBruteForce(vector<shared_ptr<Vertex>>& vertices, vector<vector<bool>>& adjMax, double k){
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
        //vertices[r].disp -= diff/diffABS*af(k,diffABS); 
      }else{
        cerr << "same position" << endl;
        vertices[i]->disp -= rf(k,10) ; 
      }
    }
  }

  for (int i=0; i<numVertices; i++){
    for (int r=0; r<i; r++){
      if (adjMax[i][r]){
        diff = vertices[i]->pos - vertices[r]->pos; 
        diffABS = diff.abs(); 
        if (diffABS!=0){
          vertices[i]->disp -= diff/diffABS*af(k,diffABS); 
          vertices[r]->disp += diff/diffABS*af(k,diffABS); 
        }
      }
    }
  }
}

bool insert(shared_ptr<Node>& node, shared_ptr<Vertex>& particle){
  //std::cerr << "crash insert" << endl;
  //std::cerr << "insert:: particle position: " << particle->pos.x << "," << particle->pos.y << endl;
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
        if (node->n->pos.x == particle->pos.x && node->n->pos.y == particle->pos.y){
          std::cerr << "same fucking pos" << endl; 
          return false; 
          particle->pos += (node->box.c2 - node->box.c1)*2; 
        }
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
  //cerr << "the tree size BY RIGHTTTT: " << vertices.size() << endl; 
 
  int numVertices = vertices.size(); 
  //init tree
  initTree(root,width,length, vertices, dynamic); 
  //root = {nullptr,nullptr, nullptr, nullptr, nullptr, {{0,0}, {width,0}, {width,length}, {0,length}}};
  //cerr << "number of iterations" << endl;
  for (int i=0; i<numVertices; i++){
    //cerr << i << " iterations=============" << endl; 
    //cerr << "start insert init tree" << endl; 
    auto result = insert(root, vertices[i]); 
    while (!result){
      vertices[i]->pos.x = fRand(0,width); 
      vertices[i]->pos.y = fRand(0,length); 
      result = insert(root, vertices[i]); 
    }
    //cerr << "wat is gg on here" << i << endl; 
    //cerr << "end insert init tree" << endl;
  } 
}

void computeMassDistribution(shared_ptr<Node>& node, double mass=1){
  //cerr << "any particle: " << node->noParticles() <<endl; 
  //cerr << "COMPUTE MASS DISTRIBUTION" << endl; 
  if (node->numChild()==0 && node->n!=nullptr){
    //cerr << "if" << endl; 
    node->mass = mass;
    node->centreOfMass.x = node->n->pos.x; 
    node->centreOfMass.y = node->n->pos.y; 
    //cerr << "compute mass distribution:" << node->mass << "|centre of mass " <<  
    //node->centreOfMass.x << "," << node->centreOfMass.y << endl; 
  }
  else{
    //cerr << "else" << endl; 
    if (node->first!=nullptr&&!node->first->noParticles()){
      computeMassDistribution(node->first, mass); 
      node->mass += node->first->mass; 
      node->centreOfMass += node->first->centreOfMass; 
      //cerr << "1. compute mass distribution:" << node->mass << "|centre of mass " <<  
      //node->centreOfMass.x << "," << node->centreOfMass.y << endl; 
    }
    if (node->second!=nullptr&&!node->second->noParticles()){
      computeMassDistribution(node->second, mass); 
      node->mass += node->second->mass; 
      node->centreOfMass += node->second->centreOfMass; 
      //cerr << "2. compute mass distribution:" << node->mass << "|centre of mass " <<  
      //node->centreOfMass.x << "," << node->centreOfMass.y << endl; 
    }
    if (node->third!=nullptr&&!node->third->noParticles()){
      computeMassDistribution(node->third, mass); 
      node->mass += node->third->mass; 
      node->centreOfMass += node->third->centreOfMass; 
     // cerr << "3. compute mass distribution:" << node->mass << "|centre of mass " <<  
      //node->centreOfMass.x << "," << node->centreOfMass.y << endl; 
    }
    if (node->fourth!=nullptr&&!node->fourth->noParticles()){
      computeMassDistribution(node->fourth, mass); 
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

MathVector calculateForceBarnesHutPerVertex(shared_ptr<Node>& node, shared_ptr<Vertex>& targetParticle, double k){
  double distance, height, theta;
  MathVector force = {0,0}; 

  //if it is a leaf
  if (node->numChild()==0&&node->n!=nullptr){
    MathVector diff = node->centreOfMass-targetParticle->pos;
    //MathVector diff = node->n->pos-targetParticle->pos; 
    distance = diff.abs(); 
    
    if (distance==0){
      //std::cerr << "distance==0" << endl; 
      return {0,0};
    }
    /*
    if (distance<0.01)
      distance = 0.01; */
    height = node->box.c2.x - node->box.c1.x ; 
    force = (diff/distance)*(node->mass*rf(k,distance));
  }
  else{
    //cerr << "target particle position: " << targetParticle->pos.x << "," << targetParticle->pos.y << endl;
    //cerr << "centre of mass " << node->centreOfMass.x << "," << node->centreOfMass.y << endl; 
    MathVector diff = node->centreOfMass-targetParticle->pos; 
    //cerr << "diff.." << diff.x <<"," <<diff.y << endl; 
    distance = diff.abs(); 
    //cerr << "distance" << distance << endl; 
    height = node->box.c2.x - node->box.c1.x ; 
    //*2 ?
    theta = height/distance; 

    /*
    if (distance<0.01)
      distance = 0.01; */
    if (distance==0){
      std::cerr << "distance==0" << endl; 
      return {0,0};
    }
       
    
    if (distance!=distance){
      //cerr << "f" << f << endl; 
      //cerr<< "distance is nan" << endl; 
      std::cerr << "distance!=distance" << endl; 
      return {0,0}; 
    }
    if (theta < 0.5){
      //cerr << "theta < 0.5" << endl;
      auto temp = diff/distance; 
      //cerr << "diff/distance" << temp.x << "," <<  temp.y<< endl; 
      force = (diff/distance)*(node->mass*rf(k,distance));
    }
    else{
      //cerr << "theta > 0.5" << endl;
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
  //std::cerr << "force::" << force.x << "," << force.y << endl; 
  return force; 
}

void calculateRepulsiveForce_barnesHutAlgo(vector<shared_ptr<Vertex>>& vertices, vector<vector<bool>>& adjMax, double k, double width, double length, double mass, bool dynamic){
  MathVector diff, force; 
  double diffABS, abs; 
  int numVertices = vertices.size(); 
  //generate tree
  //cerr << "before generate tree, size:" << numVertices << endl; 
  shared_ptr<Node> tree = make_shared<Node>();

  //variable for dynamic tree
  //std::cerr << "before generate tree.." <<  endl; 
  generateTree(vertices, width, length, tree, dynamic); 
  //std::cerr << "after generate tree.." <<  endl; 
  //cerr << "start of a iteration: "<< endl;
  //cerr << "before compute mass distribution" << endl; 
  //shared_ptr<Node> temp = std::make_shared<Node>(tree); 
  
  computeMassDistribution(tree, mass); 
  //std::cerr << "after mass distribution.." <<  endl; 

  //cerr << "before calculare force " << endl ; 
  for (int i=0; i<numVertices; i++){
    force = calculateForceBarnesHutPerVertex(tree, vertices[i], k); 
    vertices[i]->disp = force; 
    //cerr << "where the fuck is the dump?" << endl; 
  }
  //std::cerr << "aft calculate force" << endl;
}

void calculateForceBarnesHut(vector<shared_ptr<Vertex>>& vertices, vector<vector<bool>>& adjMax, double k, double width, double length, double mass, bool dynamic){ 
  calculateRepulsiveForce_barnesHutAlgo(vertices,adjMax, k, width,length, mass, dynamic); 
  calculateAttrativeForce(vertices,adjMax,k); 
}

void directedForceAlgorithm(vector<shared_ptr<Vertex>>& vertices, vector<vector<bool>>& adjMax, int L, int W, int iterations, int algoType, double mass, bool dynamic){
  int numVertices = vertices.size(); 
  int area = W*L;  
  //by right should be area/numVertices
  //double k = 1;
  double k = sqrt(area/numVertices); 
  double coeff, t; 

  MathVector diff; 
  double diffABS, abs; 
  //in each iterations
  
  t = 1; 

  /*
  vector<shared_ptr<Vertex>> spVector; 
  if (algoType!=1){
    for (int i=0; i<vertices.size();i++){
      auto temp = make_shared<Vertex>(vertices[i]); 
      spVector.push_back(temp); 
    }
  }*/

  for (int iter=0; iter<iterations; iter++){
    //cerr << "iteration.." << iter << endl; 
    if (algoType==1){
      calculateForceBruteForce(vertices, adjMax, k); 
    }
    else{
      //by default
      calculateForceBarnesHut(vertices, adjMax,k,W,L,mass, dynamic); 
    }

    //for both different algorithm, you will need this
    for (int i=0; i<vertices.size(); i++){      
      /*
      cerr << "before" << endl; 
      cerr << vertices[i]->pos.x << endl; 
      cerr << vertices[i]->pos.y << endl; 
      
      cerr << "disp:" << endl; 
      cerr << vertices[i]->disp.x << endl; 
      cerr << vertices[i]->disp.y << endl; */
      abs = vertices[i]->disp.abs(); 
      vertices[i]->pos += vertices[i]->disp/abs * min(abs, t); 

      //for static box      
      /*
      vertices[i]->pos.x = min((double)W-0.001, max((double)0.001, vertices[i]->pos.x)); 
      vertices[i]->pos.y = min((double)L-0.001, max((double)0.001, vertices[i]->pos.y)); */

      /*
      if (vertices[i]->pos.x<0 || vertices[i]->pos.x > W){
        cerr << "x go out of bound.." <<  vertices[i]->pos.x << endl; 
        vertices[i]->pos.x = fRand(0,W); 
        
      }

      if (vertices[i]->pos.y<0 || vertices[i]->pos.y > L){
        cerr << "y go out of bound.." <<  vertices[i]->pos.y << endl; 
        vertices[i]->pos.y = fRand(0,L); 
      }*/
      /*
      cerr << "after" << endl; 
      cerr << vertices[i]->pos.x << endl; 
      cerr << vertices[i]->pos.y << endl; */
      /*
      vertices[i].pos.x = min((double)W-0.001, max((double)0.001, vertices[i].pos.x)); 
      vertices[i].pos.y = min((double)L-0.001, max((double)0.001, vertices[i].pos.y)); */
      
      //cerr << "before frame.." << endl; 
      //cerr << "before" << endl; 
      //cerr << vertices[i].pos.x << endl; 
      //cerr << vertices[i].pos.y << endl; 

      /*
      if (algoType==1){
        if (vertices[i].pos.x<0 || vertices[i].pos.x > W){
          vertices[i].pos.x = fRand(0,W); 
          cerr << "x go out of bound.." << endl; 
        }

        if (vertices[i].pos.y<0 || vertices[i].pos.y > L){
          vertices[i].pos.y = fRand(0,L); 
          cerr << "y go out of bound.." << endl; 
        }
      }*/

      /*
      if (algoType==1){
        vertices[i].pos.x = min((double)W-0.001, max((double)0.001, vertices[i].pos.x)); 
        vertices[i].pos.y = min((double)L-0.001, max((double)0.001, vertices[i].pos.y)); 
      } */
      
      //cerr << "dump because of the limit frame shit" << endl; 
     // cerr << "after" << endl; 
      //cerr << vertices[i].pos.x << endl; 
     // cerr << vertices[i].pos.y << endl; 
    }

    t = cool(t); 
  }
}


#include <vector>

#include "vertex.h"

using namespace std; 

class Box{
    public: 
        MathVector c1; 
        MathVector c2; 
        MathVector c3; 
        MathVector c4; 
        
        bool in(MathVector pos){
            if (pos.x > c1.x && pos.x < c2.x){
                if (pos.y > c1.y && pos.y < c4.y){
                    return true; 
                }
            }
            return false; 
        }
}; 

class Node{
    public: 
        Vertex* n; 
        Vertex* first; 
        Vertex* second; 
        Vertex* third; 
        Vertex* fourth; 
        Box box;  


        bool noParticles(){
            if (first==nullptr && second == nullptr && third == nullptr && fourth == nullptr){
                return true; 
            }
            return false; 
        }
}; 
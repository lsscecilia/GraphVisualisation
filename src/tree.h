#include <vector>

#include "vertex.h"

//remove
#include <iostream>

using namespace std; 

class Box{
    public: 
        MathVector c1; 
        MathVector c2; 
        MathVector c3; 
        MathVector c4; 
        
        bool in(MathVector pos){
            if (pos.x >= c1.x && pos.x <= c2.x){
                if (pos.y >= c1.y && pos.y <= c4.y){
                    return true; 
                }
            }
            return false; 
        }

        //remove, there is a very high chance that might give error
        bool equal(MathVector pos){
            //shld be pos.x == c1.x && pos.y == c1.y
            if (pos.x == c1.x || pos.x == c2.x || pos.x == c3.x || pos.x == c4.x){
                if (pos.y == c1.y || pos.y == c2.y || pos.y == c3.y || pos.y == c4.y)
                    return true; 
            }
            return false; 
        }
}; 

class Node{
    public: 
        Vertex* n; 
        Node* first; 
        Node* second; 
        Node* third;  
        Node* fourth; 
        Box box;  
        MathVector centreOfMass; 
        double mass; 
        
        bool noParticles(){
            if (first==nullptr && second == nullptr && third == nullptr && fourth == nullptr && n==nullptr){
                return true; 
            }
            return false; 
        }

        int numChild(){
            int sum=0; 
            if (first!=nullptr){
                sum++; 
            }
            if (second!=nullptr){
                sum++; 
            }
            if (third!=nullptr){
                sum++;
            }
            if (fourth!=nullptr){
                sum++;
            }
            return sum; 
        }

        Node* getOnlyChild(){
            if (first!=nullptr){
                return first;  
            }
            else if (second!=nullptr){
                return second; 
            }
            else if (third!=nullptr){
                return third; 
            }
            else if (fourth!=nullptr){
                return fourth; 
            }
            return nullptr; 
        }

        Node* getQuadrant(MathVector pos){
            cerr << "mid point calculation" << endl; 
            cerr << box.c2.x  << "-" << box.c1.x << endl; 
            cerr << box.c4.y <<"-" << box.c1.y << endl;
            double xMidPoint = (box.c2.x - box.c1.x)/2 + box.c1.x; 
            double yMidPoint = (box.c4.y - box.c1.y)/2 + box.c1.y; 
            cout << "x mid point" << xMidPoint << endl; 
            cout << "y mid point" << yMidPoint << endl; 
            if (pos.x<= xMidPoint){
                if (pos.y <= yMidPoint){
                    if (first==nullptr){
                        first = new Node; 
                        *first = {nullptr, nullptr, nullptr, nullptr, nullptr, {{box.c1.x, box.c1.y}, {xMidPoint, box.c1.y}, {xMidPoint,yMidPoint}, {box.c1.x, yMidPoint}}}; 
                    }
                    cerr << "first: " << first<< endl; 
                    return first; 
                }
                else{
                    if (fourth==nullptr){
                        fourth = new Node;
                        *fourth = {nullptr, nullptr, nullptr, nullptr, nullptr, {{box.c1.x, yMidPoint}, {xMidPoint, yMidPoint}, {xMidPoint,box.c4.y}, {box.c4.x, box.c4.y}}}; 
                    }
                        
                    cerr << "fourth: " << fourth << endl;
                    return fourth; 
                }
            }
            else{
                if (pos.y <= yMidPoint){
                    if (second==nullptr){
                        second = new Node;
                        *second = {nullptr, nullptr, nullptr, nullptr, nullptr, {{xMidPoint, box.c2.y}, {box.c2.x, box.c2.y}, {box.c2.x,yMidPoint}, {xMidPoint, yMidPoint}}}; 
                    }
                    cerr << "second: " << second << endl;
                    return second; 
                }
                else{
                    if (third==nullptr){
                        third = new Node;   
                        *third = {nullptr, nullptr, nullptr, nullptr, nullptr, {{xMidPoint, yMidPoint}, {box.c3.x, yMidPoint}, {box.c3.x,box.c3.y}, {xMidPoint, box.c3.y}}}; 
                    }
                    cerr << "third: " << third << endl;    
                    return third; 
                }
            }

        }
}; 
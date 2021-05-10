#include <vector>
#include <memory>

#include "mathVector.h"

#include <iostream>

using namespace std;

struct Vertex{
  MathVector pos; 
  MathVector disp; 
}; 


class Box{
public:
	MathVector c1;
	MathVector c2;
	MathVector c3;
	MathVector c4;

	bool in(MathVector pos){
		if (pos.x >= c1.x && pos.x <= c2.x)
		{
			if (pos.y >= c1.y && pos.y <= c4.y)
			{
				return true;
			}
		}
		return false;
	}
};

class Node{
public:
	//Vertex *n;

	shared_ptr<Vertex> n; 
	//std::shared_ptr<int>    p1 = std::make_shared<int>();
	shared_ptr<Node> first; 
	shared_ptr<Node> second; 
	shared_ptr<Node> third ; 
	shared_ptr<Node> fourth; 

	Box box;
	MathVector centreOfMass;
	double mass;

	bool noParticles(){
		if (first == nullptr && second == nullptr && third == nullptr && fourth == nullptr && n == nullptr){
			return true;
		}
		return false;
	}

	int numChild(){
		int sum = 0;
		if (first != nullptr){
			sum++;
		}
		if (second != nullptr){
			sum++;
		}
		if (third != nullptr){
			sum++;
		}
		if (fourth != nullptr){
			sum++;
		}
		return sum;
	}

	shared_ptr<Node>& getOnlyChild(){
		if (first != nullptr){
			return first;
		}
		else if (second != nullptr){
			return second;
		}
		else if (third != nullptr){
			return third;
		}
		else if (fourth != nullptr){
			return fourth;
		}
		//return nullptr;
	}

	shared_ptr<Node>& getQuadrant(MathVector pos)
	{
		//cerr << "before get quadrant" << endl ; 
		double xMidPoint = (box.c2.x - box.c1.x) / 2 + box.c1.x;
		double yMidPoint = (box.c4.y - box.c1.y) / 2 + box.c1.y;
		if (pos.x <= xMidPoint){
			if (pos.y <= yMidPoint){
				if (first == nullptr){
					//cerr << "new " << endl; 
					first = make_shared<Node>(); 
					*first = {nullptr, nullptr, nullptr, nullptr, nullptr, {{box.c1.x, box.c1.y}, {xMidPoint, box.c1.y}, {xMidPoint, yMidPoint}, {box.c1.x, yMidPoint}}};
					//cerr << "after new" << endl; 
				}
				else{
					//cerr << "old" << endl; 
				}
				return first;
			}
			else{
				if (fourth == nullptr){
					//cerr << "new " << endl; 
					fourth = make_shared<Node>(); 
					*fourth = {nullptr, nullptr, nullptr, nullptr, nullptr, {{box.c1.x, yMidPoint}, {xMidPoint, yMidPoint}, {xMidPoint, box.c4.y}, {box.c4.x, box.c4.y}}};
					//cerr << "after new" << endl;
				}
				else{
					//cerr << "old" << endl; 
				}
				return fourth;
			}
		}
		else{
			if (pos.y <= yMidPoint){
				if (second == nullptr){
					//cerr << "new " << endl; 
					second = make_shared<Node>(); 
					*second = {nullptr, nullptr, nullptr, nullptr, nullptr, {{xMidPoint, box.c2.y}, {box.c2.x, box.c2.y}, {box.c2.x, yMidPoint}, {xMidPoint, yMidPoint}}};
					//cerr << "after new" << endl;
				}
				else{
					//cerr << "old" << endl; 
				}
				return second;
			}
			else{
				if (third == nullptr){
					//cerr << "new " << endl; 
					third = make_shared<Node>(); 
					*third = {nullptr, nullptr, nullptr, nullptr, nullptr, {{xMidPoint, yMidPoint}, {box.c3.x, yMidPoint}, {box.c3.x, box.c3.y}, {xMidPoint, box.c3.y}}};
					//cerr << "after new" << endl;
				}
				else{
					//cerr << "old" << endl; 
				}
				return third;
			}
		}
		//cerr << "after get quadrant" << endl; 
	}
};

struct Vertex3D{
  MathVector3D pos; 
  MathVector3D disp; 
}; 

/*
class Box3D{
public:
	MathVector3D c1;
	MathVector3D c2;
	MathVector3D c3;
	MathVector3D c4;

	Box3D(double c1x, double c1y, double c1z, double c2x, double c2y, double c2z, double c3x, double c3y, double c3z, double c4x, double c4y, double c4z): 
	c1(c1x, c1y, c1z), 
	c2(c2x, c2y, c2z),
	c3(c3x, c3y, c3z),
	c4(c4x, c4y, c4z){}

	Box3D(){}

	bool in(MathVector3D pos) {
		if (pos.x >= c1.x && pos.x <= c2.x) {
			if (pos.y >= c1.y && pos.y <= c4.y) {
        if (pos.z >= c1.z && pos.z <= c4.z) {
				  return true;
        }
			}
		}
		return false;
	}
};

class Node3D {
public:

	shared_ptr<Vertex3D> n; 
	shared_ptr<Node3D> first; 
	shared_ptr<Node3D> second; 
	shared_ptr<Node3D> third ; 
	shared_ptr<Node3D> fourth; 

	Box3D box;
	MathVector3D centreOfMass;
	double mass;

	Node3D(double c1x, double c1y, double c1z, double c2x, double c2y, double c2z, double c3x, double c3y, double c3z, double c4x, double c4y, double c4z) :
	box(c1x, c1y, c1z, c2x, c2y, c2z, c3x, c3y, c3z, c4x, c4y, c4z) {
		n = nullptr;
		first = nullptr; 
		second = nullptr;
		third = nullptr;
		fourth = nullptr;
	}

	Node3D(){}

	bool noParticles() {
		if (first == nullptr && second == nullptr && third == nullptr && fourth == nullptr && n == nullptr){
			return true;
		}
		return false;
	}

	int numChild() {
		int sum = 0;
		if (first != nullptr) {
			sum++;
		}
		if (second != nullptr) {
			sum++;
		}
		if (third != nullptr) {
			sum++;
		}
		if (fourth != nullptr) {
			sum++;
		}
		return sum;
	}

	shared_ptr<Node3D>& getOnlyChild() {
		if (first != nullptr) {
			return first;
		} else if (second != nullptr) {
			return second;
		} else if (third != nullptr) {
			return third;
		} else if (fourth != nullptr) {
			return fourth;
		}
		//return nullptr;
	}


  // how to do this for 3d?
	shared_ptr<Node3D>& getQuadrant(MathVector3D pos) {
		//cerr << "before get quadrant" << endl ; 
		double xMidPoint = (box.c2.x - box.c1.x) / 2 + box.c1.x;
		double yMidPoint = (box.c4.y - box.c1.y) / 2 + box.c1.y;
    double zMidPoint = (box.c4.z - box.c1.z) / 2 + box.c1.z;
		if (pos.x <= xMidPoint) {
			if (pos.y <= yMidPoint) {
				if (first == nullptr) {
					first = make_shared<Node3D>(box.c1.x, box.c1.y, xMidPoint, box.c1.y, xMidPoint, yMidPoint, box.c1.x, yMidPoint); 
				}
				return first;
			} else {
				if (fourth == nullptr) {
					fourth = make_shared<Node3D>(box.c1.x, yMidPoint, xMidPoint, yMidPoint, xMidPoint, box.c4.y, box.c4.x, box.c4.y); 
				}
				return fourth;
			}
		} else {
			if (pos.y <= yMidPoint) {
				if (second == nullptr) {
					second = make_shared<Node3D>(xMidPoint, box.c2.y, box.c2.x, box.c2.y, box.c2.x, yMidPoint, xMidPoint, yMidPoint); 
				}
				return second;
			} else {
				if (third == nullptr) {
					third = make_shared<Node3D>(xMidPoint, yMidPoint, box.c3.x, yMidPoint, box.c3.x, box.c3.y, xMidPoint, box.c3.y); 
				}
				return third;
			}
		}
	}
};*/
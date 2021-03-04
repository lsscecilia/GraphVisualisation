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
	Vertex *n;
	Node *first;
	Node *second;
	Node *third;
	Node *fourth;
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

	Node *getOnlyChild(){
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
		return nullptr;
	}

	Node *getQuadrant(MathVector pos)
	{
		double xMidPoint = (box.c2.x - box.c1.x) / 2 + box.c1.x;
		double yMidPoint = (box.c4.y - box.c1.y) / 2 + box.c1.y;
		if (pos.x <= xMidPoint){
			if (pos.y <= yMidPoint){
				if (first == nullptr){
					first = new Node;
					*first = {nullptr, nullptr, nullptr, nullptr, nullptr, {{box.c1.x, box.c1.y}, {xMidPoint, box.c1.y}, {xMidPoint, yMidPoint}, {box.c1.x, yMidPoint}}};
				}
				return first;
			}
			else{
				if (fourth == nullptr){
					fourth = new Node;
					*fourth = {nullptr, nullptr, nullptr, nullptr, nullptr, {{box.c1.x, yMidPoint}, {xMidPoint, yMidPoint}, {xMidPoint, box.c4.y}, {box.c4.x, box.c4.y}}};
				}
				return fourth;
			}
		}
		else{
			if (pos.y <= yMidPoint){
				if (second == nullptr){
					second = new Node;
					*second = {nullptr, nullptr, nullptr, nullptr, nullptr, {{xMidPoint, box.c2.y}, {box.c2.x, box.c2.y}, {box.c2.x, yMidPoint}, {xMidPoint, yMidPoint}}};
				}
				return second;
			}
			else{
				if (third == nullptr){
					third = new Node;
					*third = {nullptr, nullptr, nullptr, nullptr, nullptr, {{xMidPoint, yMidPoint}, {box.c3.x, yMidPoint}, {box.c3.x, box.c3.y}, {xMidPoint, box.c3.y}}};
				}
				return third;
			}
		}
	}

	void deleteTree(){
		if (first != nullptr){
			first->deleteTree();
			delete first;
		}
		if (second != nullptr){
			second->deleteTree();
			delete second;
		}
		if (third != nullptr){
			third->deleteTree();
			delete third;
		}
		if (fourth != nullptr){
			fourth->deleteTree();
			delete fourth;
		}
	}
};
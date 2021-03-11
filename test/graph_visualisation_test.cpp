#include "gtest/gtest.h"
#include <memory.h>

#include "../src/algorithm.cpp"

TEST (MathVectorTest, equalOperator){
	MathVector mv; 
	mv = 10; 
	EXPECT_EQ(10, mv.x); 
	EXPECT_EQ(10, mv.y); 
}

TEST (MathVectorTest, equalOperatorWhenAssigned){
	MathVector mv(10,10); 
	mv = 100; 
	EXPECT_EQ(100, mv.x); 
	EXPECT_EQ(100, mv.y); 
}

TEST (MathVectorTest, additionOperator){
	MathVector mv1(1,2); 
	MathVector mv2(3,4); 
	MathVector mv = mv1 + mv2; 
	EXPECT_EQ(4, mv.x); 
	EXPECT_EQ(6, mv.y); 
}

TEST (MathVectorTest, minusOperator){
	MathVector mv1(1,2); 
	MathVector mv2(3,2); 
	MathVector mv = mv1 - mv2; 
	EXPECT_EQ(-2, mv.x); 
	EXPECT_EQ(0, mv.y); 
}

TEST (MathVectorTest, multiplyOperator){
	MathVector mv1(1,2); 
	MathVector mv2(3,4); 
	MathVector mv = mv1 * mv2; 
	EXPECT_EQ(3, mv.x); 
	EXPECT_EQ(8, mv.y); 
}

TEST (MathVectorTest, multiplyByDouble){
	MathVector mv1(1,2); 
	MathVector mv = mv1*5; 
	EXPECT_EQ(5, mv.x); 
	EXPECT_EQ(10,mv.y); 
}

TEST (MathVectorTest, divisionOperator){
	MathVector mv1(1,2); 
	MathVector mv2(3,4); 
	MathVector mv = mv1/mv2; 
	EXPECT_EQ((double)1/3, mv.x); 
	EXPECT_EQ(0.5,mv.y); 
}

TEST (MathVectorTest, divideByDouble){
	MathVector mv1(6,2); 
	MathVector mv = mv1/2; 
	EXPECT_EQ(3, mv.x); 
	EXPECT_EQ(1,mv.y); 
}

TEST (MathVectorTest, plusEqualOperator){
	MathVector mv1(1,2); 
	MathVector mv2(3,4); 
	mv1 += mv2; 
	EXPECT_EQ(4, mv1.x); 
	EXPECT_EQ(6, mv1.y); 
}

TEST (MathVectorTest, minusEqualOperator){
	MathVector mv1(1,2); 
	MathVector mv2(3,4); 
	mv1 -= mv2; 
	EXPECT_EQ(-2, mv1.x); 
	EXPECT_EQ(-2, mv1.y); 
}

TEST (MathVectorTest, multiplyEqualDouble){
	MathVector mv1(1,2); 
	mv1 *= 3.0; 
	EXPECT_EQ(3, mv1.x); 
	EXPECT_EQ(6, mv1.y); 
}

TEST (MathVectorTest, divideEqualDouble){
	MathVector mv1(1,2); 
	mv1 /= 0.5; 
	EXPECT_EQ(2, mv1.x); 
	EXPECT_EQ(4, mv1.y); 
}

TEST (MathVectorTest, abs){
	MathVector mv1(1,2); 
	double abs = mv1.abs();
	EXPECT_EQ(sqrt(5), abs); 
}

TEST (MathVectorTest, min){
	MathVector mv1(10,2);
	MathVector mv = mv1.min(5); 
	EXPECT_EQ(5, mv.x); 
	EXPECT_EQ(2, mv.y);
}


TEST (Tree, generateTree){
	vector<Vertex> vertices; 
	vertices.push_back({{1,1},{0,0}}); 
	vertices.push_back({{2,2},{0,0}}); 
	vertices.push_back({{4,3},{0,0}}); 
	vertices.push_back({{5,5},{0,0}}); 
	shared_ptr<Node> tree = make_shared<Node>();
	vector<shared_ptr<Vertex>> spVertices; 
	for (int i=0;i<vertices.size();i++){
		spVertices.push_back(make_shared<Vertex>(vertices[i])); 
	}
	generateTree(spVertices, 8,8, tree ,false); 
	EXPECT_EQ(false, tree->noParticles()); 
	EXPECT_EQ(5, tree->third->n->pos.x); 
	EXPECT_EQ(5, tree->third->n->pos.y); 

	EXPECT_EQ(4, tree->first->third->n->pos.x); 
	EXPECT_EQ(3, tree->first->third->n->pos.y); 

	EXPECT_EQ(2, tree->first->first->third->n->pos.x); 
	EXPECT_EQ(2, tree->first->first->third->n->pos.y); 

	EXPECT_EQ(1, tree->first->first->first->n->pos.x); 
	EXPECT_EQ(1, tree->first->first->first->n->pos.y); 
}

TEST (Tree, generateTree2){
	vector<Vertex> vertices; 
	vertices.push_back({{1.51738,5.26204},{0,0}}); 
	vertices.push_back({{3.07885,3.23085},{0,0}}); 
	vertices.push_back({{4.01661,3.44349},{0,0}}); 
	vertices.push_back({{0.0584325,0.287978},{0,0}}); 
	vertices.push_back({{5.48902,6.29487},{0,0}}); 
	vertices.push_back({{1.56064,8.20051},{0,0}}); 
	vertices.push_back({{7.59669,1.91242},{0,0}}); 
	vertices.push_back({{7.75213,1.2933},{0,0}}); 
	vector<shared_ptr<Vertex>> spVertices; 
	for (int i=0;i<vertices.size();i++){
		spVertices.push_back(make_shared<Vertex>(vertices[i])); 
	}
		
	shared_ptr<Node> tree = make_shared<Node>();
	generateTree(spVertices, 10,10, tree, false);

	EXPECT_EQ(false, tree->noParticles()); 

	EXPECT_EQ(0.0584325,tree->first->first->n->pos.x); 
	EXPECT_EQ(0.287978,tree->first->first->n->pos.y); 

	EXPECT_EQ(3.07885, tree->first->third->first->n->pos.x);
	EXPECT_EQ(3.23085, tree->first->third->first->n->pos.y);
	
	EXPECT_EQ(4.01661, tree->first->third->second->n->pos.x);
	EXPECT_EQ(3.44349, tree->first->third->second->n->pos.y);

	EXPECT_EQ(7.75213,tree->second->second->fourth->first->n->pos.x); 
	EXPECT_EQ(1.2933,tree->second->second->fourth->first->n->pos.y); 

	EXPECT_EQ(7.59669,tree->second->second->fourth->fourth->n->pos.x); 
	EXPECT_EQ(1.91242,tree->second->second->fourth->fourth->n->pos.y); 

	EXPECT_EQ(5.48902, tree->third->n->pos.x); 
	EXPECT_EQ(6.29487, tree->third->n->pos.y); 

	EXPECT_EQ(1.51738, tree->fourth->first->n->pos.x);
	EXPECT_EQ(5.26204, tree->fourth->first->n->pos.y); 

	EXPECT_EQ(1.56064, tree->fourth->fourth->n->pos.x);
	EXPECT_EQ(8.20051, tree->fourth->fourth->n->pos.y); 
}

TEST (Tree, mass){
	//better to mock values
	vector<Vertex> vertices; 
	vertices.push_back({{1,1},{0,0}}); 
	vertices.push_back({{2,2},{0,0}}); 
	vertices.push_back({{4,3},{0,0}}); 
	vertices.push_back({{5,5},{0,0}}); 
	shared_ptr<Node> tree = make_shared<Node>();

	vector<shared_ptr<Vertex>> spVertices; 
	for (int i=0;i<vertices.size();i++){
		spVertices.push_back(make_shared<Vertex>(vertices[i])); 
	}

	generateTree(spVertices, 8,8, tree, false); 

	computeMassDistribution(tree);
	EXPECT_EQ(4 , tree->mass); 
	EXPECT_EQ(3, tree->first->mass); 
	EXPECT_EQ(2, tree->first->first->mass); 
	EXPECT_EQ(1, tree->first->first->first->mass); 
	EXPECT_EQ(1, tree->first->first->third->mass); 
	EXPECT_EQ(1, tree->first->third->mass); 
	EXPECT_EQ(1, tree->third->mass); 
}

TEST (Tree, centreOfMass){
	//better to mock values
	vector<Vertex> vertices; 
	vertices.push_back({{1,1},{0,0}}); 
	vertices.push_back({{2,2},{0,0}}); 
	vertices.push_back({{4,3},{0,0}}); 
	vertices.push_back({{5,5},{0,0}}); 
	shared_ptr<Node> tree = make_shared<Node>();

	vector<shared_ptr<Vertex>> spVertices; 
	for (int i=0;i<vertices.size();i++){
		spVertices.push_back(make_shared<Vertex>(vertices[i])); 
	}

	generateTree(spVertices, 8,8, tree, false); 

	computeMassDistribution(tree);
	EXPECT_EQ(1.5,tree->first->first->centreOfMass.x); 
	EXPECT_EQ(1.5,tree->first->first->centreOfMass.y); 
	
	EXPECT_EQ(5.5/3, tree->first->centreOfMass.x); 
	EXPECT_EQ(1.5, tree->first->centreOfMass.y); 

	EXPECT_EQ((5.5/3 + 5)/4, tree->centreOfMass.x); 
	EXPECT_EQ(1.625, tree->centreOfMass.y); 

	EXPECT_EQ(1,tree->first->first->first->centreOfMass.x); 
	EXPECT_EQ(1,tree->first->first->first->centreOfMass.y); 
}

TEST (ErrorTree, generateTree){
	vector<Vertex> vertices; 
	vertices.push_back({{3.44718,1.40869},{0,0}}); 
	vertices.push_back({{7.318,1.19414},{0,0}}); 
	vertices.push_back({{9.54089,0.993429},{0,0}}); 
	vertices.push_back({{9.45409,5.05656},{0,0}}); 
	vertices.push_back({{0.326581,1.17779},{0,0}}); 
	vertices.push_back({{7.55364,1.90332},{0,0}}); 

	shared_ptr<Node> tree = make_shared<Node>();

	vector<shared_ptr<Vertex>> spVertices; 
	for (int i=0;i<vertices.size();i++){
		spVertices.push_back(make_shared<Vertex>(vertices[i])); 
	}

	generateTree(spVertices, 10,10, tree, false);

	EXPECT_EQ(false, tree->noParticles()); 

	EXPECT_EQ(9.45409, tree->third->n->pos.x); 
	EXPECT_EQ(5.05656, tree->third->n->pos.y); 

	EXPECT_EQ(0.326581, tree->first->first->n->pos.x); 
	EXPECT_EQ(1.17779, tree->first->first->n->pos.y); 

	EXPECT_EQ(3.44718, tree->first->second->n->pos.x); 
	EXPECT_EQ(1.40869, tree->first->second->n->pos.y); 

	EXPECT_EQ(7.318, tree->second->first->n->pos.x); 
	EXPECT_EQ(1.19414, tree->second->first->n->pos.y); 

	EXPECT_EQ(9.54089, tree->second->second->second->n->pos.x); 
	EXPECT_EQ(0.993429, tree->second->second->second->n->pos.y); 

	EXPECT_EQ(7.55364, tree->second->second->fourth->n->pos.x); 
	EXPECT_EQ(1.90332, tree->second->second->fourth->n->pos.y); 

	//check all the nullptr
	EXPECT_EQ(nullptr, tree->n); 
	EXPECT_EQ(nullptr, tree->fourth); 

	EXPECT_EQ(nullptr, tree->first->n); 
	EXPECT_EQ(nullptr, tree->first->third); 
	EXPECT_EQ(nullptr, tree->first->fourth);

	EXPECT_EQ(nullptr, tree->second->n); 
	EXPECT_EQ(nullptr, tree->second->third); 
	EXPECT_EQ(nullptr, tree->second->fourth); 
	EXPECT_EQ(nullptr, tree->second->second->n); 
	EXPECT_EQ(nullptr, tree->second->second->first); 
	EXPECT_EQ(nullptr, tree->second->second->third); 
}

TEST (ErrorTree, mass){
	vector<Vertex> vertices; 
	vertices.push_back({{3.44718,1.40869},{0,0}}); 
	vertices.push_back({{7.318,1.19414},{0,0}}); 
	vertices.push_back({{9.54089,0.993429},{0,0}}); 
	vertices.push_back({{9.45409,5.05656},{0,0}}); 
	vertices.push_back({{0.326581,1.17779},{0,0}}); 
	vertices.push_back({{7.55364,1.90332},{0,0}}); 
	shared_ptr<Node> tree = make_shared<Node>();

	vector<shared_ptr<Vertex>> spVertices; 
	for (int i=0;i<vertices.size();i++){
		spVertices.push_back(make_shared<Vertex>(vertices[i])); 
	}
	generateTree(spVertices, 10,10, tree, false);

	//mass
	computeMassDistribution(tree); 

	//leaf
	EXPECT_EQ(1, tree->first->first->mass); 
	EXPECT_EQ(1, tree->first->second->mass); 
	EXPECT_EQ(1, tree->second->first->mass); 
	EXPECT_EQ(1, tree->second->second->second->mass); 
	EXPECT_EQ(1, tree->second->second->fourth->mass); 
	EXPECT_EQ(1, tree->third->mass); 

	EXPECT_EQ(3 , tree->second->mass); 
	EXPECT_EQ(2, tree->first->mass); 

	EXPECT_EQ(6, tree->mass); 
}

int main(int argc, char** argv){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
#include "gtest/gtest.h"

//#include "../src/mathVector.h"
//#include "../src/algorithm.h"
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
	Node tree = generateTree(vertices, 8,8); 
	EXPECT_EQ(false, tree.noParticles()); 
	EXPECT_EQ(5, tree.third->n->pos.x); 
	EXPECT_EQ(5, tree.third->n->pos.y); 

	EXPECT_EQ(4, tree.first->third->n->pos.x); 
	EXPECT_EQ(3, tree.first->third->n->pos.y); 

	EXPECT_EQ(2, tree.first->first->third->n->pos.x); 
	EXPECT_EQ(2, tree.first->first->third->n->pos.y); 

	EXPECT_EQ(1, tree.first->first->first->n->pos.x); 
	EXPECT_EQ(1, tree.first->first->first->n->pos.y); 
	cerr << "next test..." << endl; 
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
	Node tree = generateTree(vertices, 10,10);
	EXPECT_EQ(false, tree.noParticles()); 

	cerr << "fuck this shit" << endl; 
	
	EXPECT_EQ(0.0584325,tree.first->first->n->pos.x); 
	EXPECT_EQ(0.287978,tree.first->first->n->pos.y); 

	cerr << "here is the prob" << endl; 
	EXPECT_EQ(3.07885, tree.first->third->first->n->pos.x);
	EXPECT_EQ(3.23085, tree.first->third->first->n->pos.y);

	cerr << "?? then wtf isit?" << endl; 
	
	EXPECT_EQ(4.01661, tree.first->third->second->n->pos.x);
	EXPECT_EQ(3.44349, tree.first->third->second->n->pos.y);

	EXPECT_EQ(7.75213,tree.second->second->fourth->first->n->pos.x); 
	EXPECT_EQ(1.2933,tree.second->second->fourth->first->n->pos.y); 

	EXPECT_EQ(7.59669,tree.second->second->fourth->fourth->n->pos.x); 
	EXPECT_EQ(1.91242,tree.second->second->fourth->fourth->n->pos.y); 

	EXPECT_EQ(5.48902, tree.third->n->pos.x); 
	EXPECT_EQ(6.29487, tree.third->n->pos.y); 

	EXPECT_EQ(1.51738, tree.fourth->first->n->pos.x);
	EXPECT_EQ(5.26204, tree.fourth->first->n->pos.y); 

	EXPECT_EQ(1.56064, tree.fourth->fourth->n->pos.x);
	EXPECT_EQ(8.20051, tree.fourth->fourth->n->pos.y); 
}

int main(int argc, char** argv){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
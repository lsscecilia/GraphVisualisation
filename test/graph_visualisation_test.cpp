#include "gtest/gtest.h"

#include "../src/mathVector.h"

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

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
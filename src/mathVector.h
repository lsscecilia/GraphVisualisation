#include <math.h>

class MathVector{
  public: 
    MathVector(){}
    MathVector(double x_, double y_){
      x = x_;
      y = y_;  
    }
    ~MathVector(){

    }
    double x; 
    double y; 

    MathVector operator =(const int i){
      MathVector mv; 
      x=i; 
      y=i; 
      return mv; 
    }

    MathVector operator +(const MathVector op){
      MathVector mv; 
      mv.x = x + op.x; 
      mv.y = y + op.y; 
      return mv; 
    }
    MathVector operator -(const MathVector op){
      MathVector mv; 
      mv.x = x - op.x; 
      mv.y = y - op.y; 
      return mv; 
    }

    MathVector operator *(const MathVector op){
      MathVector mv; 
      mv.x = x*op.x; 
      mv.y = y*op.y; 
      return mv; 
    }

    MathVector operator *(const double d){
      MathVector mv; 
      mv.x = x*d; 
      mv.y = y*d; 
      return mv; 
    }
    
    MathVector operator /(const MathVector op){
      MathVector mv; 
      mv.x = x/op.x; 
      mv.y = y/op.y;
      return mv; 
    }
    
    MathVector operator /(const double d){
      MathVector mv; 
      mv.x = x/d; 
      mv.y = y/d; 
      return mv; 
    }

    void operator +=(const MathVector op){
      x += op.x; 
      y += op.y; 
    }

    void operator -=(const MathVector op){
      x -= op.x; 
      y -= op.y; 
    }

    void operator *=(const double d){
      x *= d; 
      y *= d; 
    }

    void operator /=(const double d){
      x /= d; 
      y /= d; 
    }

    double abs(){
      return sqrt(x*x+y*y); 
    }

    MathVector min(double num){
      MathVector mv; 
      if (num<x)
        mv.x = num; 
      else
        mv.x = x; 
      
      if (num<y)
        mv.y = num; 
      else 
        mv.y = y; 
      
      return mv; 
    }

    MathVector max(double num){
      MathVector mv; 
      if (num>x)
        mv.x = num; 
      else 
        mv.x = x; 
      
      if (num>y)
        mv.y = y; 
      else 
        mv.y = y; 
      
      return mv; 
    }
}; 
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

    void operator -=(const double d){
      x -= d; 
      y -= d; 
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

class MathVector3D : public MathVector{
    public: 
    MathVector3D(){}
    MathVector3D(double x_, double y_, double z_) : MathVector(x_, y_) {
      z = z_;
    }
    ~MathVector3D(){}

    double z;

    MathVector3D operator =(const int i){
      MathVector3D mv; 
      x = i; 
      y = i; 
      z = i;
      return mv; 
    }

    MathVector3D operator +(const MathVector3D op){
      MathVector3D mv; 
      mv.x = x + op.x; 
      mv.y = y + op.y; 
      mv.z = z + op.z; 
      return mv; 
    }
    MathVector3D operator -(const MathVector3D op){
      MathVector3D mv; 
      mv.x = x - op.x; 
      mv.y = y - op.y; 
      mv.z = z - op.z; 
      return mv; 
    }

    MathVector3D operator *(const MathVector3D op){
      MathVector3D mv; 
      mv.x = x*op.x; 
      mv.y = y*op.y; 
      mv.z = z*op.z; 
      return mv; 
    }

    MathVector3D operator *(const double d){
      MathVector3D mv; 
      mv.x = x*d; 
      mv.y = y*d; 
      mv.z = z*d; 
      return mv; 
    }
    
    MathVector3D operator /(const MathVector3D op){
      MathVector3D mv; 
      mv.x = x/op.x; 
      mv.y = y/op.y;
      mv.z = z/op.z;
      return mv; 
    }
    
    MathVector3D operator /(const double d){
      MathVector3D mv; 
      mv.x = x/d; 
      mv.y = y/d; 
      mv.z = z/d; 
      return mv; 
    }

    void operator +=(const MathVector3D op){
      x += op.x; 
      y += op.y; 
      z += op.z; 
    }

    void operator -=(const MathVector3D op){
      x -= op.x; 
      y -= op.y; 
      z -= op.z; 
    }

    void operator -=(const double d){
      x -= d; 
      y -= d; 
      z -= d; 
    }


    void operator *=(const double d){
      x *= d; 
      y *= d; 
      z *= d; 
    }

    void operator /=(const double d){
      x /= d; 
      y /= d; 
      z /= d; 
    }

    double abs(){
      return sqrt(x*x+y*y+z*z); 
    }

    MathVector3D min(double num){
      MathVector3D mv; 
      if (num<x)
        mv.x = num; 
      else
        mv.x = x; 
      
      if (num<y)
        mv.y = num; 
      else 
        mv.y = y; 

      if (num<z)
        mv.z = num; 
      else 
        mv.z = z; 
      
      return mv; 
    }

    MathVector3D max(double num){
      MathVector3D mv; 
      if (num>x)
        mv.x = num; 
      else 
        mv.x = x; 
      
      if (num>y)
        mv.y = y; 
      else 
        mv.y = y; 

      if (num>z)
        mv.z = z; 
      else 
        mv.z = z; 
      
      return mv; 
    }
};
